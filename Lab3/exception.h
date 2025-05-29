#pragma once

class GaussException {
 public:
  GaussException(const char* message) : message_(message) {}
  const char* what() const noexcept { return message_; }

 private:
  const char* message_;
};