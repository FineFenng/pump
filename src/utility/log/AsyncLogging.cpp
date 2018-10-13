//
// Created by finefenng on 2018/9/23.
//

#include <pump/utility/log/AsyncLogging.h>
#include <pump/utility/log/FileLogger.h>
#include <pump/Timestamp.hpp>

namespace pump {
namespace utility {

void AsyncLogging::append(const BYTE_T* log_line, size_t length) {
  std::lock_guard<std::mutex> lk(mutex_);
  if (main_buffer_->avail() > length) {
    main_buffer_->append(reinterpret_cast<const char*>(log_line), length);
  } else {
    buffer_ptr_list_.push_back(main_buffer_.release());

    if (standby_buffer_) {
      main_buffer_.reset(standby_buffer_.release());
    } else {
      main_buffer_.reset(new LargeFixedBuffer); // Rarely happens
    }
    main_buffer_->append(reinterpret_cast<const char*>(log_line), length);
    cond_.notify_one();
  }
}
void AsyncLogging::stop() {
  is_running_ = false;
  cond_.notify_one();
  thread_wrapper_.join();
}
void AsyncLogging::thread_function() {
  assert(is_running_);
  latch_.count_down();
  FileLogger fileLogger(base_name_, roll_size_, false);
  LargeFixedBuffer* buffer_ptr_1 = new LargeFixedBuffer();
  LargeFixedBuffer* buffer_ptr_2 = new LargeFixedBuffer();
  buffer_ptr_1->bzero();
  buffer_ptr_2->bzero();

  BufferVector written_buffers;
  written_buffers.reserve(16);

  while (is_running_) {
    PUMP_ASSERT(buffer_ptr_1 && buffer_ptr_1->length() == 0);
    PUMP_ASSERT(buffer_ptr_2 && buffer_ptr_2->length() == 0);
    PUMP_ASSERT(written_buffers.empty());

    {
      std::unique_lock<std::mutex> lk(mutex_);
      if (!buffer_ptr_list_.empty()) {
        cond_.wait_for(lk, std::chrono::seconds(flush_interval_));
      }
      buffer_ptr_list_.push_back(main_buffer_.release());
      main_buffer_.reset(buffer_ptr_1);
      buffer_ptr_1 = nullptr;
      written_buffers.swap(buffer_ptr_list_);
      if (!standby_buffer_) {
        standby_buffer_.reset(buffer_ptr_2);
        buffer_ptr_2 = nullptr;
      }
    }

    PUMP_ASSERT(!written_buffers.empty());
    if (written_buffers.size() > 25) {
      char buffer[256] = {0};
      snprintf(buffer, sizeof(buffer), "Dropped log messages at %s, %zd larger buffers\n",
               Timestamp::now().get_time_point_string().c_str(),
               written_buffers.size() - 2);
      fputs(buffer, stderr);
      fileLogger.append(reinterpret_cast<BYTE_T*>(buffer), strlen(buffer));
      written_buffers.erase(written_buffers.begin() + 2, written_buffers.end());
    }

    for (size_t i = 0; i < written_buffers.size(); ++i) {
      fileLogger.append(reinterpret_cast<const BYTE_T*>(written_buffers[i]->data()), written_buffers[i]->length());
    }
    if (written_buffers.size() > 2) {
      // drop non-bzero-ed buffers, avoid trashing
      written_buffers.resize(2);
    }

    if (!buffer_ptr_1) {
      PUMP_ASSERT(!written_buffers.empty());
      buffer_ptr_1 = written_buffers.back();
      written_buffers.pop_back();
      buffer_ptr_1->reset();
    }

    if (!buffer_ptr_2) {
      PUMP_ASSERT(!written_buffers.empty());
      buffer_ptr_2 = written_buffers.back();
      written_buffers.pop_back();
      buffer_ptr_2->reset();
    }

    written_buffers.clear();
    fileLogger.flush();
  }

}
}

}

