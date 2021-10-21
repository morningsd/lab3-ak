#include <iostream>
#include <map>
#include <string.h>
#include <iterator>
#include <vector>
#include <algorithm>
#include <sstream>
#include <ctype.h>

using namespace std;

void parse_short(int pos, char *argv[]);
void parse_long(int pos, char *argv[]);
void command_output(string command);
vector<string> split(string str, string token);
vector<int> convert_to_int_vec(const vector<string>& vec);
bool is_number(string word);

map<string, char> commands;
map<string, char>::iterator map_it;
string::iterator str_it;

vector<string> parsed_commands;

string list_opts;
vector<int> list_opts_int;
string value_opt;
int value_opt_int;

int main(int argc, char *argv[])
{
  string delimiter = "-";
  
  commands["help"] = 'h';
  commands["h"] = 'h';
  commands["list"] = 'l';
  commands["l"] = 'l';
  commands["value"] = 'v';
  commands["v"] = 'v';
  
  for (int i = 1; i < argc; ++i) {
    string arg = argv[i];
    if (arg.find("--") == 0) {
      parse_long(i, argv);
    } else if (arg.find("-") == 0) {
      parse_short(i, argv);
    }
  }
  for (string s : parsed_commands) {
    command_output(s);
  }
  return 0;
}


void parse_short(int pos, char *argv[])
{
  string arg = argv[pos];
  for (str_it = arg.begin() + 1; str_it != arg.end(); ++str_it) {
    char curr = *str_it;
    string curr_str(1, curr);
    if (commands.count(curr_str)) {
      if (find(parsed_commands.begin(), parsed_commands.end(), curr_str) == parsed_commands.end()) {
	parsed_commands.push_back(curr_str);
	if (curr_str == "l") {
	  list_opts = arg.substr(str_it + 1 - arg.begin());
	  break;
	} else if (curr_str == "v") {
	  value_opt = arg.substr(str_it + 1 - arg.begin());
	  break;
	}
      } else {
	if (curr_str == "l" || curr_str == "v") break;
      }
    } else {
      cout << "Unknown command: " << arg << '\n';
      break;
    }
  }
}

void parse_long(int pos, char *argv[])
{
  string arg = argv[pos];
  string s = arg.substr(2);
  if (commands.count(s)) {
    char curr_value = commands[s];
    string curr_value_str(1, curr_value);
    if (find(parsed_commands.begin(), parsed_commands.end(), curr_value_str) == parsed_commands.end()) {
      parsed_commands.push_back(curr_value_str);
      if (curr_value_str == "l" && (strcmp(argv[pos + 1], "=") == 0)) {
	list_opts = argv[pos + 2];
      } else if (curr_value_str == "v" && (strcmp(argv[pos + 1], "=") == 0)) {
	value_opt = argv[pos + 2];
      }
    }
  } else {
    cout << "Unknown command: " << arg << '\n';
  }
}

void command_output(string command)
{
  if (command == "v") {
    if (!is_number(value_opt)) {
      cout << "Value: wrong argument: " << value_opt << '\n';
    } else {
      stringstream sstr(value_opt);
      sstr >> value_opt_int;
      cout << "Value = " << value_opt_int << '\n';
    }
  } else if (command == "h") {
    cout << "No help today :L\n";
  } else if (command == "l") {
    vector<string> list_opts_str = split(list_opts, ",");
    list_opts_int = convert_to_int_vec(list_opts_str);
    cout << "List: ";
    for (int i : list_opts_int) {
      cout << i << " ";
    }
    cout << '\n';
  } else {
    cout << "Unknown argument";
  }
}

vector<string> split(string str, string token)
{
    vector<string>result;
    while(str.size()) {
        int index = str.find(token);
        if (index != -1) {
	  result.push_back(str.substr(0,index));
          str = str.substr(index+token.size());
          if (str.size() == 0) result.push_back(str);
        } else {
          result.push_back(str);
          str = "";
        }
    }
    return result;
}

vector<int> convert_to_int_vec(const vector<string>& vec)
{
  vector<int> result;
  for (size_t i = 0; i < vec.size(); ++i) {
    if (vec[i].length() != 0) {
      string word = vec[i];
      if (!is_number(word)) continue;
      stringstream sstr(word);
      int value;
      sstr >> value;
      result.push_back(value);
    }
  }
  return result;
}

bool is_number(string word)
{
  bool is_number = true;
  for(string::const_iterator k = word.begin(); k != word.end(); ++k) {
    is_number = is_number && isdigit(*k);
  }
  return is_number;
}
