#pragma once
#include "constexpr_map.hpp"

namespace clib_util
{
	namespace hotkeys
	{
		namespace details
		{
			static constexpr std::array<std::pair<std::string_view, uint32_t>, 121> allKeys{
				{ { "Esc"sv, 1 },
					{ "1"sv, 2 },
					{ "2"sv, 3 },
					{ "3"sv, 4 },
					{ "4"sv, 5 },
					{ "5"sv, 6 },
					{ "6"sv, 7 },
					{ "7"sv, 8 },
					{ "8"sv, 9 },
					{ "9"sv, 10 },
					{ "0"sv, 11 },
					{ "-"sv, 12 },
					{ "="sv, 13 },
					{ "Backspace"sv, 14 },
					{ "Tab"sv, 15 },
					{ "Q"sv, 16 },
					{ "W"sv, 17 },
					{ "E"sv, 18 },
					{ "R"sv, 19 },
					{ "T"sv, 20 },
					{ "Y"sv, 21 },
					{ "U"sv, 22 },
					{ "I"sv, 23 },
					{ "O"sv, 24 },
					{ "P"sv, 25 },
					{ "["sv, 26 },
					{ "]"sv, 27 },
					{ "Enter"sv, 28 },
					{ "Ctrl"sv, 29 },
					{ "Control"sv, 29 },
					{ "A"sv, 30 },
					{ "S"sv, 31 },
					{ "D"sv, 32 },
					{ "F"sv, 33 },
					{ "G"sv, 34 },
					{ "H"sv, 35 },
					{ "J"sv, 36 },
					{ "K"sv, 37 },
					{ "L"sv, 38 },
					{ ";"sv, 39 },
					{ "'"sv, 40 },
					{ "`"sv, 41 },
					{ "Shift"sv, 42 },
					{ "\\"sv, 43 },
					{ "Z"sv, 44 },
					{ "X"sv, 45 },
					{ "C"sv, 46 },
					{ "V"sv, 47 },
					{ "B"sv, 48 },
					{ "N"sv, 49 },
					{ "M"sv, 50 },
					{ ","sv, 51 },
					{ "."sv, 52 },
					{ "/"sv, 53 },
					{ "RShift"sv, 54 },
					{ "RightShift"sv, 54 },
					{ "Num*"sv, 55 },
					{ "Alt"sv, 56 },
					{ "Space"sv, 57 },
					{ "CapsLock"sv, 58 },
					{ "F1"sv, 59 },
					{ "F2"sv, 60 },
					{ "F3"sv, 61 },
					{ "F4"sv, 62 },
					{ "F5"sv, 63 },
					{ "F6"sv, 64 },
					{ "F7"sv, 65 },
					{ "F8"sv, 66 },
					{ "F9"sv, 67 },
					{ "F10"sv, 68 },
					{ "NumLock"sv, 69 },
					{ "ScrollLock"sv, 70 },
					{ "Num7"sv, 71 },
					{ "Num8"sv, 72 },
					{ "Num9"sv, 73 },
					{ "Num-"sv, 74 },
					{ "Num4"sv, 75 },
					{ "Num5"sv, 76 },
					{ "Num6"sv, 77 },
					{ "Num+"sv, 78 },
					{ "NumPLUS"sv, 78 },  // Num+ alias
					{ "Num1"sv, 79 },
					{ "Num2"sv, 80 },
					{ "Num3"sv, 81 },
					{ "Num0"sv, 82 },
					{ "NumDel"sv, 83 },
					{ "SysReq"sv, 84 },
					// { "\\"sv, 86 }, // This one is duplicated. Need to check.
					{ "F11"sv, 87 },
					{ "F12"sv, 88 },
					{ "F13"sv, 124 },
					{ "F14"sv, 125 },
					{ "F15"sv, 126 },
					{ "F16"sv, 127 },
					{ "F17"sv, 128 },
					{ "F18"sv, 129 },
					{ "F19"sv, 130 },
					{ "F20"sv, 131 },
					{ "F21"sv, 132 },
					{ "F22"sv, 133 },
					{ "F23"sv, 134 },
					{ "F24"sv, 135 },
					{ "NumEnter"sv, 156 },
					{ "RCtrl"sv, 157 },
					{ "RightCtrl"sv, 157 },
					{ "RightControl"sv, 157 },
					{ "RControl"sv, 157 },
					{ "Num/"sv, 181 },
					{ "RAlt"sv, 184 },
					{ "RightAlt"sv, 184 },
					{ "Home"sv, 199 },
					{ "Up"sv, 200 },
					{ "PageUp"sv, 201 },
					{ "Left"sv, 203 },
					{ "Right"sv, 205 },
					{ "End"sv, 207 },
					{ "Down"sv, 208 },
					{ "PageDown"sv, 209 },
					{ "Insert"sv, 210 },
					{ "Ins"sv, 210 },
					{ "Del"sv, 211 },
					{ "Delete"sv, 211 } }
			};

			inline uint32_t GetKeyByName(std::string_view name)
			{
				static constexpr auto keyMap = constexpr_map<std::string_view, uint32_t, allKeys.size()>{ { allKeys } };
				return keyMap.at(name);
			}

			inline std::string_view GetNameByKey(uint32_t key)
			{
				static constexpr auto keyMap = constexpr_map<std::string_view, uint32_t, allKeys.size()>{ { allKeys } };
				return keyMap.key(key);
			}
		}

		struct KeyCombination
		{
			using Key = uint32_t;
			using Trigger = std::function<void(const KeyCombination*)>;

			KeyCombination(Trigger trigger) :
				trigger(trigger){};

			KeyCombination(std::string pattern, Trigger trigger) :
				KeyCombination(trigger)
			{
				SetPattern(pattern);
			}

			KeyCombination(std::set<Key> keys, Trigger trigger) :
				KeyCombination(trigger)
			{
				std::vector<std::string> rawKeys;

				try {
					std::transform(keys.begin(), keys.end(), std::inserter(rawKeys, rawKeys.begin()), [](const auto& key) {
						auto name = details::GetNameByKey(key);
						return std::string(name == "NumPLUS"sv ? "Num+"sv : name);
					});
				} catch (std::range_error&) {
					isValid = false;
				}

				this->keys = keys;
				this->pattern = string::join(rawKeys, " + ");
			}

			bool Process(RE::InputEvent* const* a_event)
			{
				if (!isValid)
					return false;

				std::set<Key> pressed;
				for (auto event = *a_event; event; event = event->next) {
					if (event->eventType != RE::INPUT_EVENT_TYPE::kButton)
						continue;

					const auto button = static_cast<RE::ButtonEvent*>(event);
					if (!button) {
						continue;
					}

					if (!button->HasIDCode()) {
						continue;
					}

					if (button->IsPressed()) {
						pressed.insert(button->idCode);
					}
				}

				if (pressed == keys) {
					if (!alreadyTriggered) {
						alreadyTriggered = true;
						trigger(this);
					}
				} else {
					alreadyTriggered = false;
				}

				return alreadyTriggered;
			}

			std::string_view GetPattern() const
			{
				return pattern;
			}

			bool SetPattern(std::string_view pattern)
			{
				auto str = string::capitalize(pattern, '+');

				string::replace_all(str, " "sv, ""sv);
				string::replace_all(str, "Num+"sv, "NumPLUS"sv);

				std::set<Key> keys{};
				std::vector<std::string> rawKeys = string::split(str, "+");

				try {
					std::ranges::transform(rawKeys, std::inserter(keys, keys.begin()), [](const auto& raw) {
						return details::GetKeyByName(raw);
					});

					rawKeys.clear();
					std::transform(keys.begin(), keys.end(), std::inserter(rawKeys, rawKeys.begin()), [](const auto& key) {
						auto name = details::GetNameByKey(key);
						return std::string(name == "NumPLUS"sv ? "Num+"sv : name);
					});
				} catch (std::range_error&) {
					isValid = false;
					return false;
				}

				this->keys = keys;
				this->pattern = string::join(rawKeys, " + ");
				isValid = true;
				return true;
			}

			bool IsValid() const
			{
				return isValid;
			}

			bool IsTriggered()
			{
				return alreadyTriggered;
			}

		private:
			Trigger trigger;
			std::string pattern;
			std::set<Key> keys;
			bool alreadyTriggered = false;
			bool isValid = false;
		};
	}
}
