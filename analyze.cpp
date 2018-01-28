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
	std::ifstream f("test.java");
	std::vector<token> v_token;
	std::string line;
	bool in_comment = false;
	while(std::getline(f, line)) {
		// word may contains the punctuations like ;,(){}
		std::string word;
		std::istringstream line_stream(line);
		while(line_stream >> word) {
			std::cout << word << std::endl;

			// Better approach is to read the line char by char.
			std::string s;
			std::string::size_type i = 0;
			while(i < word.size()) {
				if (word[i] == ';') {
					push_back_token(v_token, word[i], "semi-colon");
					push_back_token(v_token, s, "variable");
					s.clear();
					i++;
					continue;
				} else if (word[i] == ',') {
					push_back_token(v_token, word[i], "comma");
					push_back_token(v_token, s, "variable");
					s.clear();
					i++;
					continue;
				} else if (word[i] == '{') {
					push_back_token(v_token, word[i], "open-curly-brace");
					push_back_token(v_token, s, "variable");
					s.clear();
					i++;
					continue;
				} else if (word[i] == '}') {
					push_back_token(v_token, word[i], "close-curly-brace");
					push_back_token(v_token, s, "variable");
					s.clear();
					i++;
					continue;
				} else if (word[i] == '(') {
					push_back_token(v_token, word[i], "open-parenthesis");
					push_back_token(v_token, s, "variable");
					s.clear();
					i++;
					continue;
				} else if (word[i] == ')') {
					push_back_token(v_token, word[i], "close-parenthesis");
					push_back_token(v_token, s, "variable");
					s.clear();
					i++;
					continue;
				} else if (word[i] == ':') {
					push_back_token(v_token, word[i], "colon");
					push_back_token(v_token, s, "variable");
					s.clear();
					i++;
					continue;
				}

				s.append(1, word[i]);

				i++;
				// if (s == comment_inline) {
				// 	push_back_token(v_token, s, "inline-comment");
				// 	s.clear();
				// 	break;// can break because this is inner while loop is the parsing BY LINE;
				// } else if (s == comment_begin) {
				// 	push_back_token(v_token, s, "open-comment");
				// 	s.clear();
				// 	in_comment = true;
				// } else if (s == comment_end) {
				// 	push_back_token(v_token, s, "open-comment");
				// 	s.clear();
				// 	in_comment = false;
				// } else {

				// }
			}

			if (in_comment)
				continue;

		}
	}

	for (auto t : v_token)
		std::cout << t;

	f.close();
	return 0;
}

