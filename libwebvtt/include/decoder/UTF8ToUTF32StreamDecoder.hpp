#ifndef LIBWEBVTT_INCLUDE_DECODER_UTF8_TO_UTF32_STREAM_DECODER_HPP_
#define LIBWEBVTT_INCLUDE_DECODER_UTF8_TO_UTF32_STREAM_DECODER_HPP_

#include "buffer/StringBuffer.hpp"
#include "buffer/StringSyncBuffer.hpp"
#include <memory>
#include <string>
#include <thread>
#include <utility>


namespace webvtt {

class UTF8ToUTF32StreamDecoder {

 public:
  explicit UTF8ToUTF32StreamDecoder(std::shared_ptr<StringBuffer < char8_t>>
  inputStream);

  UTF8ToUTF32StreamDecoder() = default;
  UTF8ToUTF32StreamDecoder(const UTF8ToUTF32StreamDecoder &) = delete;
  UTF8ToUTF32StreamDecoder(UTF8ToUTF32StreamDecoder &&) = delete;
  UTF8ToUTF32StreamDecoder &operator=(const UTF8ToUTF32StreamDecoder &) = delete;
  UTF8ToUTF32StreamDecoder &operator=(UTF8ToUTF32StreamDecoder &&) = delete;
  ~UTF8ToUTF32StreamDecoder();

  /**
   * Start decoding if not already started
   * @return true if decoding successfully started.
   * If already started return false
   */
  bool startDecoding();

  /**
   *
   * @return Pointer to buffer that contain decoded data
   */
  std::shared_ptr<StringSyncBuffer < char32_t>> getDecodedStream();

 private:
  constexpr static int DEFAULT_READ_NUMBER = 10;
  bool decodingStarted = false;

  std::shared_ptr<StringBuffer < char8_t>> inputStream;
  std::shared_ptr<StringSyncBuffer < char32_t>> outputStream;

  std::unique_ptr<std::thread> decoderThread;

  /**
   * Try to convert given string to utf32 string.
   * Part of string that is successfully converted are erased.
   * @param readBytes string to be converted
   * @return
   */
  static std::u32string decodeReadBytes(std::u8string &readBytes);

  /**
   * Use as run method for thread that is decoding input stream.
   */
  void decodeInputStream();
};
}

#endif // LIBWEBVTT_INCLUDE_DECODER_UTF8_TO_UTF32_STREAM_DECODER_HPP_