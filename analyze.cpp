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
 * and the Variable `Scope`.
 *
 * Need to consider how to display the error.
 *
 */

static std::string comment_inline = "//";
static std::string comment_begin = "/*";
static std::string comment_end = "*/";


int main() {
	std::ifstream f("test.java");
	std::string line;
	bool in_comment = false;
	while(std::getline(f, line)) {
		std::string word;
		std::istringstream line_stream(line);
		while(line_stream >> word) {
			// Better approach is to read the line char by char.
			if (word.find(comment_inline) == 0) {
				break;
			}
			if (word.find(comment_begin) == 0) {
				in_comment = true;
			}
			if (word.find(comment_end) != std::string::npos) {
				// Please note that some bad code like `/*\n*/some code`.
				// find a way to handle this.
				in_comment = false;
			}
			if (in_comment)
				continue;
			std::cout << word << std::endl;
			continue;
		}
	}

	f.close();
	return 0;
}

