#pragma once
#include "constexpr_map.hpp"

namespace clib_util
{
	namespace hotkeys
	{
		namespace details
		{
			static constexpr std::array<std::pair<std::string_view, uint32_t>, 121> allKeys{
				{ { "esc"sv, 1 },
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
					{ "backspace"sv, 14 },
					{ "tab"sv, 15 },
					{ "q"sv, 16 },
					{ "w"sv, 17 },
					{ "e"sv, 18 },
					{ "r"sv, 19 },
					{ "t"sv, 20 },
					{ "y"sv, 21 },
					{ "u"sv, 22 },
					{ "i"sv, 23 },
					{ "o"sv, 24 },
					{ "p"sv, 25 },
					{ "["sv, 26 },
					{ "]"sv, 27 },
					{ "enter"sv, 28 },
					{ "ctrl"sv, 29 },
					{ "control"sv, 29 },
					{ "a"sv, 30 },
					{ "s"sv, 31 },
					{ "d"sv, 32 },
					{ "f"sv, 33 },
					{ "g"sv, 34 },
					{ "h"sv, 35 },
					{ "j"sv, 36 },
					{ "k"sv, 37 },
					{ "l"sv, 38 },
					{ ";"sv, 39 },
					{ "'"sv, 40 },
					{ "`"sv, 41 },
					{ "shift"sv, 42 },
					{ "\\"sv, 43 },
					{ "z"sv, 44 },
					{ "x"sv, 45 },
					{ "c"sv, 46 },
					{ "v"sv, 47 },
					{ "b"sv, 48 },
					{ "n"sv, 49 },
					{ "m"sv, 50 },
					{ ","sv, 51 },
					{ "."sv, 52 },
					{ "/"sv, 53 },
					{ "rshift"sv, 54 },
					{ "rightshift"sv, 54 },
					{ "num*"sv, 55 },
					{ "alt"sv, 56 },
					{ "space"sv, 57 },
					{ "capslock"sv, 58 },
					{ "f1"sv, 59 },
					{ "f2"sv, 60 },
					{ "f3"sv, 61 },
					{ "f4"sv, 62 },
					{ "f5"sv, 63 },
					{ "f6"sv, 64 },
					{ "f7"sv, 65 },
					{ "f8"sv, 66 },
					{ "f9"sv, 67 },
					{ "f10"sv, 68 },
					{ "numlock"sv, 69 },
					{ "scrolllock"sv, 70 },
					{ "num7"sv, 71 },
					{ "num8"sv, 72 },
					{ "num9"sv, 73 },
					{ "num-"sv, 74 },
					{ "num4"sv, 75 },
					{ "num5"sv, 76 },
					{ "num6"sv, 77 },
					{ "num+"sv, 78 },
					{ "numplus"sv, 78 },  // num+ alias
					{ "num1"sv, 79 },
					{ "num2"sv, 80 },
					{ "num3"sv, 81 },
					{ "num0"sv, 82 },
					{ "numdel"sv, 83 },
					{ "sysreq"sv, 84 },
					// { "\\"sv, 86 }, // this one is duplicated. need to check.
					{ "f11"sv, 87 },
					{ "f12"sv, 88 },
					{ "f13"sv, 124 },
					{ "f14"sv, 125 },
					{ "f15"sv, 126 },
					{ "f16"sv, 127 },
					{ "f17"sv, 128 },
					{ "f18"sv, 129 },
					{ "f19"sv, 130 },
					{ "f20"sv, 131 },
					{ "f21"sv, 132 },
					{ "f22"sv, 133 },
					{ "f23"sv, 134 },
					{ "f24"sv, 135 },
					{ "numenter"sv, 156 },
					{ "rctrl"sv, 157 },
					{ "rightctrl"sv, 157 },
					{ "rightcontrol"sv, 157 },
					{ "rcontrol"sv, 157 },
					{ "num/"sv, 181 },
					{ "ralt"sv, 184 },
					{ "rightalt"sv, 184 },
					{ "home"sv, 199 },
					{ "up"sv, 200 },
					{ "pageup"sv, 201 },
					{ "left"sv, 203 },
					{ "right"sv, 205 },
					{ "end"sv, 207 },
					{ "down"sv, 208 },
					{ "pagedown"sv, 209 },
					{ "insert"sv, 210 },
					{ "ins"sv, 210 },
					{ "del"sv, 211 },
					{ "delete"sv, 211 } }
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
						return std::string(name == "numplus"sv ? "num+"sv : name);
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
				auto str = string::tolower(pattern);

				string::replace_all(str, " "sv, ""sv);
				string::replace_all(str, "num+"sv, "numplus"sv);

				std::set<Key> keys{};
				std::vector<std::string> rawKeys = string::split(str, "+");

				try {
					std::ranges::transform(rawKeys, std::inserter(keys, keys.begin()), [](const auto& raw) {
						return details::GetKeyByName(raw);
					});

					rawKeys.clear();
					std::transform(keys.begin(), keys.end(), std::inserter(rawKeys, rawKeys.begin()), [](const auto& key) {
						auto name = details::GetNameByKey(key);
						return std::string(name == "numplus"sv ? "num+"sv : name);
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
