#pragma once
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>

using LocalizationMap = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;

// This parses a line from a .csv and also accounts for quotes and commas sinside quotes. Referenced from a medium user known as Ryan.
// Adapted to allow for human-readable tokens.
inline std::vector<std::string> parseCSVLine(const std::string& line) {
	std::vector<std::string> result;
	std::string field;
	bool InQuotes = false;

	for (size_t i = 0; i < line.size(); ++i) {
		char c = line[i];

		if (c == '"') {
			if (InQuotes && i + 1 < line.size() && line[i + 1] == '"') {
				field += '"';
				++i;
			}
			else {
				InQuotes = !InQuotes;
			}
		}
		else if (c == ',' && !InQuotes) {
			result.push_back(field);
			field.clear();
		}
		else if (c == '\\' && InQuotes && i + 1 < line.size() && line[i + 1] == 'n') {
			field += '\n'; // convert \n to actual newline
			++i; // skip 'n'
		}
		else {
			field += c;
		}
	}

	result.push_back(field);
	return result;
}

inline LocalizationMap loadLocalizationCSV(const std::string& filename) {
	std::ifstream file(filename);
	std::string line;
	LocalizationMap translations;

	std::vector<std::string> headers;
	bool firstLine = true;

	while (std::getline(file, line)) {
		if (line.empty()) continue;

		std::vector<std::string> fields = parseCSVLine(line);
		if (firstLine) {
			headers = fields;
			firstLine = false;
			continue;
		}

		std::string token = fields[0];
		for (size_t i = 1; i < fields.size(); ++i) {
			if (i < headers.size()) {
				std::string lang = headers[i];
				translations[token][lang] = fields[i];
			}
		}
	}

	return translations;
}

// Lookup.
inline std::string getLocalizedString(const LocalizationMap& map, const std::string& token, const std::string& lang) {
	auto tokenIt = map.find(token);
	if (tokenIt != map.end()) {
		auto langIt = tokenIt->second.find(lang);
		if (langIt != tokenIt->second.end()) {
			return langIt->second;
		}
	}
	//If we can't find an equivalent we return the token in brackets instead.
	return "[[" + token + "]]";
}

inline std::string WideToUtf8(const std::wstring& wstr) {
	if (wstr.empty()) return {};

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(),
		static_cast<int>(wstr.size()),
		nullptr, 0, nullptr, nullptr);

	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(),
		static_cast<int>(wstr.size()),
		&strTo[0], size_needed, nullptr, nullptr);

	return strTo;
}

//Gets the End User's UI Language.
inline std::string getUserUILanguage()
{
	LANGID langID = GetUserDefaultUILanguage();
	LCID lcid = MAKELCID(langID, SORT_DEFAULT);
	WORD primaryLang = PRIMARYLANGID(langID);
	wchar_t localeName[LOCALE_NAME_MAX_LENGTH]{};
	int result = LCIDToLocaleName(lcid, localeName, LOCALE_NAME_MAX_LENGTH, 0);

	return WideToUtf8(localeName);

}


//class LocalizationManager {
//public:
//    bool loadFromCSV(const std::string& filename);
//    void setLanguage(const std::string& langCode);
//    std::string get(const std::string& token) const;
//
//private:
//    std::string currentLanguage;
//    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> translations;
//};
//
//inline bool LocalizationManager::loadFromCSV(const std::string& filename) {
//    std::ifstream file(filename);
//    if (!file.is_open()) return false;
//
//    std::string line;
//    std::getline(file, line);
//    std::stringstream ss(line);
//
//    // Parse header for languages
//    std::vector<std::string> languages;
//    std::string lang;
//    std::getline(ss, lang, ','); // skip "Token" header
//    while (std::getline(ss, lang, ',')) {
//        languages.push_back(lang);
//    }
//
//    while (std::getline(file, line)) {
//        std::stringstream row(line);
//        std::string token;
//        std::getline(row, token, ',');
//
//        for (size_t i = 0; i < languages.size(); ++i) {
//            std::string translation;
//            if (!std::getline(row, translation, ',')) break;
//            translations[token][languages[i]] = translation;
//        }
//    }
//
//    return true;
//}
//
//inline void LocalizationManager::setLanguage(const std::string& langCode) {
//    currentLanguage = langCode;
//}
//
//inline std::string LocalizationManager::get(const std::string& token) const {
//    auto tokenIt = translations.find(token);
//    if (tokenIt != translations.end()) {
//        const auto& langMap = tokenIt->second;
//        auto langIt = langMap.find(currentLanguage);
//        if (langIt != langMap.end()) {
//            return langIt->second;
//        }
//    }
//    return "[[" + token + "]]";  // Fallback if not found
//}