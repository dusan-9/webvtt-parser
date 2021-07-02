#include "parser/ParserUtil.h"
#include "exception"

namespace WebVTT {

    //TODO GIVE NAMES TO CONSTANTS
    bool ParserUtil::isSpaceCharacter(uint32_t character) {
        if (character >= 0x0009 && character <= 0x000D)
            return true;

        if (character >= 0x2000 && character <= 0x200A)
            return true;

        uint32_t checkList[9] = {
                0x0085, 0x0020, 0x00A0, 0x1680, 0x2028, 0x2029, 0x202F, 0x205F, 0x3000};
        auto position = std::find(std::begin(checkList), std::end(checkList), character);
        return position != std::end(checkList);
    }

    inline bool ParserUtil::isDigit(uint32_t character) {
        return character >= 0x0030 && character <= 0x0039;
    }

    std::optional<std::string>
    ParserUtil::collectDigits(std::u32string_view input, std::u32string_view::iterator &position) {
        if (position < input.begin())
            return std::nullopt;
        std::string collectedDigits;
        while (ParserUtil::isDigit(*position) && position != input.end()) {
            std::u32string temp{*position};
            collectedDigits.append(utf8::utf32to8(temp));
            position++;
        }
        return collectedDigits;
    }

    //TODO Change implementation to use stoi or collect digits
    std::optional<double>
    ParserUtil::parsePercentage(std::u32string_view input) {
        return parseFloatPointingNumber(input.substr(0, input.length()));
    }

    std::optional<double>
    ParserUtil::parseFloatPointingNumber(std::u32string_view input) {
        try {
            return std::stod(utf8::utf32to8(input));
        }
        catch (const std::out_of_range &e) {
            return std::nullopt;
        }
        catch (const std::invalid_argument &e) {
            return std::nullopt;
        }
    }

    bool ParserUtil::skipWhiteSpaces(std::u32string_view input, std::u32string_view::iterator &position) {
        if (position < input.begin())
            return false;
        while (ParserUtil::isSpaceCharacter(*position) && position != input.end()) {
            std::advance(position, 1);
        }
        return position == input.end();
    }

    bool ParserUtil::stringEqualStringPlusSpaces(std::u32string_view string, std::u32string_view lookFor) {
        auto position = string.find(lookFor, 0);
        if (position != 0)
            return false;
        auto positionIter = string.begin() + position;
        skipWhiteSpaces(string, positionIter);
        return positionIter == string.end();
    }

    bool ParserUtil::stringContainsSeparator(std::u32string_view input, std::u32string_view separator) {
        //TODO - maybe change to string and use regex
        //Change to use find
        return input.find(separator, 0) != std::u32string_view::npos;
    };

    bool ParserUtil::compareU32Strings(std::u32string_view str1, std::u32string_view str2) {
        return str1.compare(str2) == 0;
    }

    constexpr std::u32string_view TIME_STAMP_SEPARATOR = U"-->";

    std::u32string_view
    ParserUtil::parseUntilCharacter(std::u32string_view input, uint32_t character,
                                    std::u32string_view::iterator &position) {
        auto startPosition = position - input.begin();
        auto endPosition = input.find(character, startPosition);

        position = std::next(position);
        return input.substr(startPosition, endPosition);
    }

    std::optional<std::tuple<std::u32string_view, std::u32string_view>>
    ParserUtil::splitStringAroundCharacter(std::u32string_view input, uint32_t character) {

        auto charPos = input.find(character);

        //TODO Check this for all useges
        // if (charPos == std::u32string_view::npos || charPos == 0 || charPos == input.length())
        // {
        //     return std::nullopt;
        // }

        if (charPos == std::u32string_view::npos) {
            return std::nullopt;
        }
        if (charPos == 0)
            return std::make_tuple<>(std::u32string_view(), input);

        if (charPos == input.length() - 1)
            return std::make_tuple<>(input, std::u32string_view());

        auto key = input.substr(0, charPos);
        auto value = input.substr(charPos + 1, input.length());
        return std::make_tuple<>(key, value);
    }

    std::optional<std::tuple<double, double>>
    ParserUtil::parseCoordinates(std::u32string_view coordinates, uint32_t separator) {
        auto splitDataOptional = ParserUtil::splitStringAroundCharacter(coordinates, separator);
        if (!splitDataOptional.has_value())
            return std::nullopt;
        auto[xCoordinateString, yCoordinateString] = splitDataOptional.value();
        if (xCoordinateString.empty() || yCoordinateString.empty())
            return std::nullopt;

        double xCoord, yCoord;


        auto xCordOptional = ParserUtil::parsePercentage(xCoordinateString);
        if (!xCordOptional.has_value()) return std::nullopt;

        auto yCoordOptional = ParserUtil::parsePercentage(yCoordinateString);
        if (!yCoordOptional.has_value()) return std::nullopt;


        return std::make_tuple(xCordOptional.value(), yCoordOptional.value());
    }


};
