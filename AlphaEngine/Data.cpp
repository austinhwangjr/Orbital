/******************************************************************************/
/*!
\file		Data.cpp
\author		Leong Wen Bin, Aceson (100%)
\par    	email: leong.w\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definitions of functions to read data from 
			text file

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
 /******************************************************************************/

#include "Data.h"

int ImportDataFromFile(const char* FileName, std::vector<Data>& Data_vector, std::map<std::string, f32>& Datamap){

	std::ifstream ifs{ FileName, std::ios_base::in };
	if (!ifs.is_open()) {											// Check if file exist/open	
		std::cout << FileName << "does not exist." << '\n';
		return 0;
	}

	std::string line;
	while (std::getline(ifs, line)) {
		Data Node;
		std::string word;
		int find_word = 1;

		for (char const ch : line) {

			if (find_word) {
				if (ch == '/') {
					break;
				}

				if (ch == '\n') {
					break;
				}

				if (ch == ' ' || ch == '\t') {
					if (!word.empty()) {    // if ch is a whitespace and word contains some letters
						Node.variable_name = word;
						find_word = 0;
						word.clear();
					}
				}
				else {
					word.push_back(ch);

				}
			}
			else if (!find_word) {
				word.push_back(ch);
			}
		}

		if (find_word == 0) {
			Node.data = word;
			Data_vector.push_back(Node);
		}

	}

	for (size_t i = 0; i < Data_vector.size(); i++) {
		Datamap[Data_vector[i].variable_name] = std::stof(Data_vector[i].data);
	}

	ifs.close();

	return 1;
}
