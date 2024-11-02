#include "video_processor.hpp"
#include <add_noise.hpp>
void VideoProcessor::process() {
  cv::Mat frame, processed_frame;
  int frame_number = 0;
  ProgressBar progress(frame_count_);

  if (!readAndInitializeFirstFrame(frame)) {
    return;
  }

  // Process frames
  while (cap_.read(frame)) {

    try {
      processFrame(frame);
      output_.write(processed_frame);
      progress.update(++frame_number);
    } catch (const cv::Exception &e) {
      std::cerr << "\nError processing frame " << frame_number << ": "
                << e.what() << std::endl;
      break;
    }
  }

  finalizeProcessing();
}

void VideoProcessor::initializeVideo() {
  if (!cap_.isOpened()) {
    throw std::runtime_error("Failed to open video: " +
                             std::string(input_path_));
  }

  frame_count_ = static_cast<int>(cap_.get(cv::CAP_PROP_FRAME_COUNT));
  fps_ = cap_.get(cv::CAP_PROP_FPS);
  frame_width_ = static_cast<int>(cap_.get(cv::CAP_PROP_FRAME_WIDTH));
  frame_height_ = static_cast<int>(cap_.get(cv::CAP_PROP_FRAME_HEIGHT));

  printVideoProperties();
}

void VideoProcessor::initializeWriter() {
  std::filesystem::create_directories(OUTPUT_DIR);

  const std::vector<int> codecs = {cv::VideoWriter::fourcc('H', '2', '6', '4'),
                                   cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                                   cv::VideoWriter::fourcc('X', 'V', 'I', 'D'),
                                   cv::VideoWriter::fourcc('D', 'I', 'V', 'X')};

  bool writer_initialized = false;
  for (const auto &codec : codecs) {
    output_.open(OUTPUT_FILE, codec, fps_,
                 cv::Size(frame_width_, frame_height_), true);

    if (output_.isOpened()) {
      std::cout << "Using codec: " << static_cast<char>(codec & 0xFF)
                << static_cast<char>((codec >> 8) & 0xFF)
                << static_cast<char>((codec >> 16) & 0xFF)
                << static_cast<char>((codec >> 24) & 0xFF) << std::endl;
      writer_initialized = true;
      break;
    }
  }

  if (!writer_initialized) {
    throw std::runtime_error("Failed to create output video file");
  }
}

void VideoProcessor::processFrame(const cv::Mat &input) {

  if (isCpu_) {
    cv::Mat output;
    output = stkmbCpu_->processFrame(input);
    output_.write(output);

  } else {

    frameGpu_.upload(input);

    // Convert to grayscale if necessary
    if (input.channels() == 3) {
      cv::cuda::cvtColor(frameGpu_, outputGpu_, cv::COLOR_BGR2GRAY);
      frameGpu_ = outputGpu_;
    }

    // Process with denoiser
    stkmbGpu_->process(frameGpu_, outputGpu_);

    cv::Mat output;
    outputGpu_.download(output);

    cv::Mat colorOutput;
    cv::cvtColor(output, colorOutput, cv::COLOR_GRAY2BGR);

    output_.write(colorOutput);
  }
}

bool VideoProcessor::readAndInitializeFirstFrame(cv::Mat &frame) {
  if (!cap_.read(frame)) {
    throw std::runtime_error("Failed to read first frame");
    return false;
  }

  if (frame.empty()) {
    throw std::runtime_error("First frame is empty");
    return false;
  }
  // Upload frame to GPU
  frameGpu_.upload(frame);

  if (frame.channels() == 3) {
    cv::cuda::cvtColor(frameGpu_, outputGpu_, cv::COLOR_BGR2GRAY);
    frameGpu_ = outputGpu_; // Store grayscale version
  }
  if (isCpu_) {
    stkmbCpu_ = std::make_unique<STKMBCpu>(frame);
  } else {
    stkmbGpu_ = std::make_unique<STKMBGpu>(frameGpu_);
  }
  return true;
}

void VideoProcessor::cleanup() {
  if (cap_.isOpened()) {
    cap_.release();
  }
  if (output_.isOpened()) {
    output_.release();
  }
}

void VideoProcessor::finalizeProcessing() {
  output_.release();

  std::filesystem::path output_path(OUTPUT_FILE);
  if (!std::filesystem::exists(output_path)) {
    throw std::runtime_error("Output file was not created");
  }

  auto file_size = std::filesystem::file_size(output_path);
  std::cout << "\nProcessing complete! Output saved to " << OUTPUT_FILE
            << "\nFile size: " << file_size << " bytes" << std::endl;
}

void VideoProcessor::printVideoProperties() const {
  std::cout << "Video properties:\n"
            << "  Resolution: " << frame_width_ << "x" << frame_height_ << "\n"
            << "  FPS: " << fps_ << "\n"
            << "  Frame count: " << frame_count_ << "\n"
            << "Starting processing..." << std::endl;
}