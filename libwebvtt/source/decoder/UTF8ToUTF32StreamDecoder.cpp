#include "decoder/UTF8ToUTF32StreamDecoder.hpp"
#include "parser/ParserUtil.hpp"
#include "utf8.h"
#include <thread>
#include <utility>
#include "logger/LoggingUtility.hpp"
#include "buffer/StringSyncBuffer.hpp"

namespace webvtt {
UTF8ToUTF32StreamDecoder::UTF8ToUTF32StreamDecoder(std::shared_ptr<StringBuffer<char8_t>> newInputStream)
    : inputStream(std::move(newInputStream)) {
  outputStream = std::make_shared<StringSyncBuffer<char32_t>>();
}

std::u32string UTF8ToUTF32StreamDecoder::decodeReadBytes(std::u8string &readBytes) {
  auto positionInvalid = ParserUtil::find_invalid(readBytes);

  auto validUTF8 = std::u8string_view(readBytes);
  std::u32string utf_32;

  if (positionInvalid == std::string::npos) {
    utf_32 = ParserUtil::utf8to32(validUTF8);
    readBytes.clear();
  } else {
    utf_32 = ParserUtil::utf8to32(validUTF8.substr(0, positionInvalid));
    readBytes.erase(readBytes.begin(), readBytes.begin() + positionInvalid);
  }
  return utf_32;
}

void UTF8ToUTF32StreamDecoder::decodeInputStream() {
  std::u8string buffer;
  std::u8string bytes;

  try {

    while (true) {
      bytes = inputStream->readMultiple(DEFAULT_READ_NUMBER);

      if (bytes.length() == 0) {
        break;
      }
      buffer.append(bytes);
      std::u32string decodedBytes = decodeReadBytes(buffer);

      outputStream->writeMultiple(decodedBytes);
    }
    outputStream->setInputEnded();
  }
  catch (const std::bad_alloc &error) {
    DILOGE(error.what());
    outputStream->setInputEnded();
    inputStream->clearBufferUntilReadPosition();
    throw;
  }
};

bool UTF8ToUTF32StreamDecoder::startDecoding() {
  if (decodingStarted)
    return false;
  decodingStarted = true;
  decoderThread = std::make_unique<std::thread>(&UTF8ToUTF32StreamDecoder::decodeInputStream, this);
  return true;
};

std::shared_ptr<StringSyncBuffer<char32_t>> UTF8ToUTF32StreamDecoder::getDecodedStream() {
  if (not decodingStarted)
    return nullptr;
  return outputStream;
};

UTF8ToUTF32StreamDecoder::~UTF8ToUTF32StreamDecoder() {
  if (not decodingStarted)
    return;
  decoderThread->join();
  decodingStarted = false;
}
}