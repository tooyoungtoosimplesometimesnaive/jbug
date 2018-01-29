#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>

/*
 *
 * The key point is to analyze the code,
 * Find out which is the `Variable`, and the corresponding `Type`.
 * and the Variable `Scope` or `Context`.
 *
 * Need to consider how to display the error.
 *
 */

static std::string comment_inline = "//";
static std::string comment_begin = "/*";
static std::string comment_end = "*/";

struct token {
	std::string literal;
	std::string type;
	token(std::string l, std::string t) : literal(l), type(t) {}
	friend std::ostream& operator<<(std::ostream& os, const token& t);
};

std::ostream& operator<<(std::ostream& os, const token& t) {
	os << t.literal << " " << t.type << std::endl;
	return os;
}

/*
	T is something that can be converted to string.
*/
template<typename T>
void push_back_token(std::vector<token> & v, T token_literal, std::string type) {
	std::string tl;
	/* Only assignment for char is good.*/
	tl = token_literal;
	if (tl.size() != 0) {
		token t(tl, type);
		v.push_back(t);
	}
}

int main() {
	std::ifstream f("test_2.java");
	std::vector<token> v_token;
	std::string line;
	bool in_comment = false, inline_comment = false, in_string_literal = false;

	while(std::getline(f, line)) {
		// word may contains the punctuations like ;,(){}
		std::string word;
		std::string string_literal;
		std::istringstream line_stream(line);
		while(line_stream >> word) {
			std::cout << "~~~~~~" << word << std::endl;

			// Better approach is to read the line char by char.
			std::string s;
			std::string::size_type i = 0;
			while(i < word.size()) {
				if (!in_comment && !inline_comment) {
					if (word[i] == ';') {
						push_back_token(v_token, s, "variable");
						s.clear();
						push_back_token(v_token, word[i], "semi-colon");
						i++;
						continue;
					} else if (word[i] == ',') {
						push_back_token(v_token, s, "variable");
						s.clear();
						push_back_token(v_token, word[i], "comma");
						i++;
						continue;
					} else if (word[i] == '{') {
						push_back_token(v_token, s, "variable");
						s.clear();
						push_back_token(v_token, word[i], "open-curly-brace");
						i++;
						continue;
					} else if (word[i] == '}') {
						push_back_token(v_token, s, "variable");
						s.clear();
						push_back_token(v_token, word[i], "close-curly-brace");
						i++;
						continue;
					} else if (word[i] == '(') {
						push_back_token(v_token, s, "variable");
						s.clear();
						push_back_token(v_token, word[i], "open-parenthesis");
						i++;
						continue;
					} else if (word[i] == ')') {
						push_back_token(v_token, s, "variable");
						s.clear();
						push_back_token(v_token, word[i], "close-parenthesis");
						i++;
						continue;
					} else if (word[i] == ':') {
						push_back_token(v_token, s, "variable");
						s.clear();
						push_back_token(v_token, word[i], "colon");
						i++;
						continue;
					} else if (word[i] == '"' && !in_string_literal) {
						push_back_token(v_token, s, "variable");
						s.clear();
						string_literal.append(1, '"');
						in_string_literal = true;
						i++;
						continue;
					} else if (word[i] == '"' && in_string_literal) {
						string_literal.append(1, '"');
						push_back_token(v_token, string_literal, "string_literal");
						in_string_literal = false;
						string_literal.clear();
						i++;
					}

					if (word[i] == '/' && !in_string_literal) {
						if (i + 1 < word.size() && word[i + 1] == '/') {
							push_back_token(v_token, s, "variable");
							s.clear();
							inline_comment = true;
							i += 2;
							continue;

						} else if (i + 1 < word.size() && word[i + 1] == '*') {
							push_back_token(v_token, s, "variable");
							s.clear();
							in_comment = true;
							i += 2;
							continue;
						}
					}
				}

				if (!inline_comment && !in_string_literal) {
					if (word[i] == '*') {
						if (i + 1 < word.size() && word[i + 1] == '/') {
							in_comment = false;
							i += 2;
							continue;
						}
					}
				}

				if (in_string_literal) {
					string_literal.append(1, word[i]);
				} else {
					s.append(1, word[i]);
				}

				i++;

				if (i == word.size() && !inline_comment && !in_comment && !in_string_literal) {
					push_back_token(v_token, s, "variable");
					s.clear();
				}
			}

			if (in_string_literal) {
				string_literal.append(" ");// Adding a single space is just a simulation.
			}


			if (in_comment)
				continue;

		}

		inline_comment = false;

	}

	for (auto t : v_token)
		std::cout << t;

	f.close();
	return 0;
}

