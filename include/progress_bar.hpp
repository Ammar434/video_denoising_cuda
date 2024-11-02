#pragma once
#include <chrono>
#include <iomanip>
#include <iostream>

class ProgressBar
{
public:
    ProgressBar(int total, int width = 50)
        : total_(total), width_(width),
          last_update_(std::chrono::steady_clock::now()) {}

    void update(int current)
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                           now - last_update_)
                           .count();

        // Update at most every 100ms to avoid flooding the output
        if (elapsed < 100 && current != total_ && current != 0)
        {
            return;
        }
        last_update_ = now;

        float progress = static_cast<float>(current) / total_;
        int filled_width = static_cast<int>(width_ * progress);

        // Calculate speed and ETA
        auto total_elapsed =
            std::chrono::duration_cast<std::chrono::seconds>(now - start_time_)
                .count();
        float frames_per_second =
            (total_elapsed > 0) ? static_cast<float>(current) / total_elapsed : 0;
        int eta_seconds =
            (frames_per_second > 0)
                ? static_cast<int>((total_ - current) / frames_per_second)
                : 0;

        // Clear the current line
        std::cout << "\r" << std::string(last_line_length_, ' ') << "\r";

        // Build progress bar
        std::cout << "[";
        for (int i = 0; i < width_; ++i)
        {
            if (i < filled_width)
                std::cout << "=";
            else if (i == filled_width)
                std::cout << ">";
            else
                std::cout << " ";
        }

        // Calculate percentage
        int percent = static_cast<int>(progress * 100);

        // Format the status line
        std::stringstream status;
        status << "] " << std::setw(3) << percent << "% " << current << "/"
               << total_ << " frames | " << std::fixed << std::setprecision(1)
               << frames_per_second << " fps | "
               << "ETA: " << formatTime(eta_seconds);

        std::cout << status.str();
        std::cout.flush();

        // Store the length of the line for the next update
        last_line_length_ = width_ + 2 + status.str().length();

        // New line on completion
        if (current == total_)
        {
            std::cout << std::endl;
        }
    }

private:
    std::string formatTime(int seconds)
    {
        int hours = seconds / 3600;
        int minutes = (seconds % 3600) / 60;
        seconds = seconds % 60;

        std::stringstream ss;
        if (hours > 0)
        {
            ss << hours << "h ";
        }
        if (minutes > 0 || hours > 0)
        {
            ss << minutes << "m ";
        }
        ss << seconds << "s";
        return ss.str();
    }

    int total_;
    int width_;
    size_t last_line_length_ = 0;
    std::chrono::steady_clock::time_point last_update_ =
        std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point start_time_ =
        std::chrono::steady_clock::now();
};