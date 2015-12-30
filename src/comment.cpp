#include <cctype>
#include <iostream>
#include "comment.h"

CFPC::CFPC():fcontent(""), isFileOpened(false) {}

CFPC::CFPC(const std::string& filename):fcontent(filename),
                              isFileOpened((fcontent.size()) ? true : false) {}

bool is_filename_correct(char* filename)
{
        std::string fname(filename);
        std::vector<std::string> vhn; // vector with hiders names
        vhn.push_back(".h");
        vhn.push_back(".hpp");
        vhn.push_back(".hxx");
        for(int i = 0; i < 3; i++) {
                size_t pos = fname.find(vhn[i]);
                if(pos != std::string::npos)
                        return true;
        }
        return false;
}
// delete all comments existing in the file
void CFPC::delete_comments()
{
        size_t pos = 0, pos1 = 0;
        while( true ) {
                pos = fcontent.find("//", pos);
                if(pos == std::string::npos)
                        break;
                pos1 = fcontent.find('\n', pos);
                fcontent.erase(pos, pos1-pos);
        }
        pos = 0;
        while( true ) {
                pos = fcontent.find("/*", pos);
                if(pos == std::string::npos)
                        break;
                pos1 = fcontent.find("*/", pos);
                fcontent.erase(pos, pos1-pos+2);
        }
}
// check is the posission is in any comment
bool CFPC::is_in_comment(size_t pos) const
{
        size_t pos1 = pos, pos2 = 0;
        pos1 = find_back('\n', pos);
        pos2 = fcontent.find("//", pos1);
        if(pos2 < pos)
                return true;
        pos1 = pos2 = -1;
        while( true ) {
                pos1 = fcontent.find("/*", pos2+1);
                if((pos1 > pos) || (pos1 == std::string::npos))
                        break;
                pos2 = fcontent.find("*/", pos1);
                if((pos > pos1) && (pos < pos2))
                        return true;
        }
        return false;
}
// check is the posission is in the constant string
bool CFPC::is_in_string(size_t pos) const
{
        unsigned int count = 0, pos1 = -1;
        if(('\'' == fcontent[pos1-1]) || ('\"' == fcontent[pos1-1]))
                return true;
        while( true ) {
                pos1 = fcontent.find('\"', pos1+1);
                if((pos1 > pos) || (pos1 == std::string::npos))
                        break;
                count++;
        }
        if(1 == count % 2)
                return true;
        return false;
}
size_t CFPC::find(const char ch, size_t pos) const
{
        size_t pos1 = pos-1;
        while( true ) {
                pos1 = fcontent.find(ch, pos1+1);
                if(!is_in_string(pos1) && !is_in_comment(pos1))
                        break;
        }
        return pos1;
}
size_t CFPC::find(const char* chp, size_t pos) const
{
        size_t pos1 = pos-1;
        while( true ) {
                pos1 = fcontent.find(chp, pos1+1);
                if(!is_in_string(pos1) && !is_in_comment(pos1))
                        break;
        }
        return pos1;
}
size_t CFPC::find(std::string str, size_t pos) const
{
        return find(str.c_str(), pos);
}
std::string CFPC::get_name_in_line(size_t begin, size_t end) const
{
        bool isword = true;
        std::string word_p;
        if(end <= begin)
                return word_p;
        while(isspace(fcontent[begin]))
                begin++;
        size_t word_begin = begin;
        while((isalpha(fcontent[begin]) || isdigit(fcontent[begin]) ||
               ('_' == fcontent[begin]) || ('*' == fcontent[begin]) ||
               ('~' == fcontent[begin]) || (':' == fcontent[begin]) ||
               ('&' == fcontent[begin])) && (end >= begin))
                begin++;

        std::string word = fcontent.substr(word_begin, begin - word_begin);
        while(end > begin) {
                if(isgraph(fcontent[begin]) && (',' != fcontent[begin]))  {
                        if(isgraph(fcontent[begin]) && ('.' != fcontent[begin])) {
                                if(isgraph(fcontent[begin]) && (';' != fcontent[begin])) {
                                        if(isgraph(fcontent[begin]) && ('*' != fcontent[begin])) {
                                                if(isgraph(fcontent[begin]) && ('&' != fcontent[begin])) {
                                                        isword = false;
                                                        break;
                                                }
                                        }
                                }
                        }
                }
                begin++;
        }
        if(isword)
                word_p = (word[word.size()-1] == ':') ? word.substr(0, word.size()-1) : word;
        return word_p;
}
size_t CFPC::find_back(const char ch, size_t pos) const
{
        for(int index = pos; index >= 0; index--)
                if(ch == fcontent[index])
                        return index;
        return std::string::npos;
}
size_t CFPC::find_bracket_couple(const char c_ch, size_t bracket_pos) const
{
        bracket_pos = find(c_ch, bracket_pos);
        size_t pos1 = bracket_pos-1, pos2 = bracket_pos+1, count = 0;
        std::string s_ch(1, c_ch);
        std::string ch_couple = (s_ch == "{") ? "}" :
                                ((s_ch == "<") ? ">" :
                                ((s_ch == "(") ? ")" : "q"));
        while(true) {
                pos1 = find(ch_couple, pos1);
                pos2 = find(c_ch, pos2);
                if(pos1 > pos2) {
                        pos2 += 1;
                        count++;
                }
                else
                        if(count) {
                                count--;
                                pos1 += 1;
                        }
                        else
                                break;
        }
        return pos1;
}
std::string CFPC::get_struct_name(size_t struct_pos) const
{
        size_t pos2 = fcontent.find_first_of(";:{", struct_pos);
        size_t pos1 = pos2;
        while(isspace(fcontent[--pos1]));
        while(isgraph(fcontent[pos1]))
                pos1--;
        std::string struct_name = get_name_in_line(pos1, pos2);
        if(struct_name.size()) {
                return struct_name;
        }
        else {
                pos1 = find_bracket_couple('{', pos2);
                pos2 = find(';', pos1);
                struct_name = get_name_in_line(pos1, pos2);
                return struct_name;
        }
}
std::string CFPC::complex_type_name(size_t pos) const
{
        std::string type_name;
        size_t pos1 = fcontent.find_first_of(";,()", pos);
        size_t temp_pos = find("<", pos);
        if(temp_pos < pos1) {
                size_t pos2 = find_bracket_couple('<', temp_pos);
                size_t pos3 = fcontent.find_first_of(";()", pos);
                if(pos2 < pos3)
                        type_name = fcontent.substr(pos, pos2-pos+1);
        }
        return type_name;
}
std::vector<std::string> type_()
{
        std::vector<std::string> types;
        types.push_back("const");
        types.push_back("long");
        types.push_back("short");
        types.push_back("static");
        types.push_back("double");
        return types;
}
std::string CFPC::get_type_name(size_t pos) const
{
        size_t count = 0, pos1 = 0;
        std::vector<std::string> types = type_();
        std::string type_name;
        while(isspace(fcontent[pos]))
                pos++;
        while( true ) {
                std::string _type_name = complex_type_name(pos);
                if(_type_name.size()) {
                        type_name += _type_name;
                        pos1 = pos + _type_name.size();
                        break;
                }
                pos1 = fcontent.find_first_of(" ),", pos);
                std::string type_n = get_name_in_line(pos, pos1);
                type_name += type_n;
                for(int i = 0; i < 5; i++)
                        if(types[i] == type_n)
                                count = 1;
                if(count) {
                        count = 0;
                        pos = pos1+1;
                        type_name += ' ';
                }
                else
                        break;
        }
        // check for pointers and
        size_t pos2 = find('[', pos1);
        size_t pos3 = find('&', pos1);
        pos = find('*', pos1);
        pos1 = fcontent.find_first_of(";,)(=abcdefghijklmnopqrstuvwxyz", pos1);
        if(pos2 < pos1) type_name += '*';
        if(pos < pos1) type_name += '*';
        if(pos3 < pos1) type_name += '&';
        return type_name;
}
size_t max_(size_t a, size_t b)
{
        if(std::string::npos == a)
                return b;
        if(std::string::npos == b)
                return a;
        return (a > b) ? a : b;
}
// for parameter it takes position of ';' or '='
std::string CFPC::get_variable_name(size_t pos) const
{
        std::string variable_name;
        pos -= 1;
        while(isspace(fcontent[pos]))
                pos--;
        size_t end_line = find('\n', pos);
        size_t start_line = find_back('\n', pos);
        size_t pos1 = fcontent.find_last_of("({", end_line);
        if((pos1 < end_line) && (pos1 > start_line))
                return variable_name;
        std::string variable_t = get_type_name(start_line);
        if((variable_t == "return") ||
           (variable_t == "delete") ||
           (variable_t == "delete*"))
                return variable_name;
        while(isspace(fcontent[start_line]))
                start_line++;
        size_t vname_pos = start_line;
        vname_pos = find("<", vname_pos);
        if( vname_pos < pos)
                vname_pos = 1 + find_bracket_couple('<', vname_pos);
        else
                vname_pos = find(' ', start_line);
        size_t comma_pos = find(',', vname_pos);
        while(isspace(fcontent[vname_pos]) || (fcontent[vname_pos] == '*')
                                           || (fcontent[vname_pos] == '&'))
                vname_pos++;
        if(comma_pos < pos) {
                while( comma_pos < pos ) {
                        variable_name += get_name_in_line(vname_pos, comma_pos);
                        vname_pos = comma_pos+1;
                        comma_pos = find(',', comma_pos+1);
                        variable_name += ", ";
                }
        }
        variable_name += get_name_in_line(vname_pos, pos);
        return variable_name;
}
std::string CFPC::put_variable_comment(size_t pos, std::string var_type, std::string var_name)
{
        size_t pos1 = pos;
        while('\n' != fcontent[pos1])
                pos1--;
        std::string tab = fcontent.substr(pos1+1, pos-pos1-1);
        std::string comment = "\n";
        comment += tab + "/*\n";
        comment += tab + "*   ! varriable - " + var_name + "\n";
        comment += tab + "*   ! type - " + var_type + "\n";
        comment += tab + "*\n";
        comment += tab + "*/\n";
        comment += tab;
        fcontent.insert(pos, comment);
        return comment;
}
std::map<size_t, std::string> CFPC::put_struct_variables_comments
                                (size_t struct_pos, std::string struct_name)
{
        size_t pos = struct_pos;
        size_t begin = find('{', struct_pos);
        size_t end = find_bracket_couple('{', begin);

        std::map<size_t, std::string> mmap;
        std::string vname;
        while( true ) {
                pos = find(';', begin);
                begin = pos+1;
                if((pos > end) || (pos == std::string::npos)) break;
                std::string var_n = get_variable_name(pos);
                if(var_n.size()) {
                        while(isspace(fcontent[--pos]));
                        pos = find_back('\n', pos);
                        while(isspace(fcontent[++pos]));
                        std::string comment = put_variable_comment(pos,
                                        get_type_name(pos-1), struct_name+"::"+var_n);
                        size_t sz = comment.size();
                        end += sz;
                        begin += sz;
                        pos += sz;
                        mmap[pos] = var_n;
                }
        }
        return mmap;
}
std::string arrange_stname_in_fname(std::string stname, std::string fname)
{
        size_t pos = fname.find_last_of(' ');
        size_t pos1 = fname.find("operator");
        if(pos1 != std::string::npos)
                fname.insert(pos1, stname + "::");
        else if(pos == std::string::npos)
                fname = stname + "::" + fname;
        else
                fname.insert(pos+1, stname + "::");
        return fname;
}


std::multimap<size_t, std::string> CFPC::put_struct_funcs_comments
                                (std::string struct_name, size_t struct_pos)
{
        bool is_func = false;
        std::string fname;
        std::multimap<size_t, std::string> fnmap;
        size_t begin = find('{', struct_pos);
        size_t end = find_bracket_couple('{', begin);

        while( true ) {
                size_t bracket_pos = find('(', begin);
                begin = bracket_pos+1;
                if(bracket_pos > end) break;

                size_t fname_pos = bracket_pos;
                while(isspace(fcontent[--fname_pos]));
                while(isgraph(fcontent[fname_pos]))
                        fname_pos--;
                size_t func_pos_x = fcontent.find_last_of("<\n)=({", bracket_pos-1);
                size_t func_pos = func_pos_x + 1;
                while(func_pos < fname_pos) {
                        if(isgraph(fcontent[func_pos])) {
                                is_func = true;
                                break;
                        }
                        func_pos++;
                }
                size_t ret_type_pos = func_pos;
                if(fcontent[func_pos_x] == '<') {
                        func_pos = fcontent.find_last_of('\n', func_pos);
                        while(isspace(fcontent[func_pos]))
                                func_pos++;
                        size_t template_pos = find("template", func_pos);
                        if(template_pos < bracket_pos) {
                                ret_type_pos = find_bracket_couple('<', func_pos);
                                fname = fcontent.substr(template_pos, ret_type_pos - func_pos+1) + ' ';
                                ret_type_pos++;
                        }
                        else
                                ret_type_pos = func_pos;

                }
                if(((fcontent[func_pos_x] == '\n') || (fcontent[func_pos_x] == '<')) && is_func) {
                        size_t oper_pos = find("operator", func_pos);
                        if(oper_pos < bracket_pos)
                                fname += fcontent.substr(oper_pos, bracket_pos-(oper_pos));
                        else
                                fname += get_name_in_line(fname_pos, bracket_pos);
                        size_t second_bracket_pos = find(')', bracket_pos);
                        size_t line_end_pos = fcontent.find_first_of("\n;{", second_bracket_pos);
                        std::string const_ = get_name_in_line(second_bracket_pos+1, line_end_pos);
                        if(const_.size())
                                fname = const_ + ' ' + fname;
                        if( std::string::npos == fname.find(struct_name) ){
                                ret_type_pos = (ret_type_pos == oper_pos) ? std::string::npos : ret_type_pos;
                                std::string comment = put_comment_ff(func_pos, get_type_name(ret_type_pos),
                                                arrange_stname_in_fname(struct_name, fname),
                                                get_func_params(bracket_pos));
                                size_t sz = comment.size();
                                begin += sz;
                                end += sz;
                                fnmap.insert(std::pair<size_t, std::string>(func_pos+sz, fname));
                        }
                        is_func = false;
                }
                else{
                        fname = get_name_in_line(fname_pos, bracket_pos);
                        if(fname.size()) {
                                std::string deconst = (fname == struct_name) ? "constructor" :
                                                      ((struct_name == fname.substr(1, fname.size()-1)) ?
                                                       "destructor" : "" );
                                if(deconst.size()){
                                        std::string comment = put_comment_ff(func_pos+1, "",
                                                        deconst, get_func_params(bracket_pos));
                                        size_t sz = comment.size();
                                        begin += sz;
                                        end += sz;
                                        fnmap.insert(std::pair<size_t, std::string>(func_pos+sz, fname));
                                }
                        }
                }
                fname.clear();
        }
        return fnmap;
}
std::multimap<size_t, std::string> CFPC::put_funcs_comments
                                (size_t begin, size_t end, std::string nname)
{
        nname = (nname.size()) ? nname + "::" : nname;
        std::multimap<size_t, std::string> fnmap;
        std::string fname;
        bool is_func = false;
        while( true ) {
                size_t pos = find('(', begin);
                begin = pos+1;
                if(pos > end) break;

                while(isspace(fcontent[--pos]));
                size_t pos1 = pos;
                while(isgraph(fcontent[--pos1]));
                fname = get_name_in_line(pos1, pos);

                pos = 1 + fcontent.find_last_of(";}", pos1);
                while(pos < pos1) {
                        if(isgraph(fcontent[pos])) {
                                is_func = true;
                                break;
                        }
                        pos++;
                }
                if( is_func ) {
                        size_t pos10 = fcontent.find_last_of(";{=", pos);
                        if(fcontent[pos10] != ';')
                                is_func = false;
                }
                if(fname.size()){
                        if( is_func ) {
                                std::string comment = put_comment_ff(pos, get_type_name(pos),
                                                                nname + fname, get_func_params(pos));
                                size_t sz = comment.size();
                                begin += sz;
                                end += sz;
                                is_func = false;
                                fnmap.insert(std::pair<size_t, std::string>(pos+sz, fname));
                        }
                }
        }
        return fnmap;
}
std::string CFPC::get_func_params(size_t func_pos) const
{
        std::string params;
        size_t pos1 = find('(', func_pos);
        size_t pos2 = find(')', func_pos);
        while( true ) {
                params += get_type_name(pos1+1);
                pos1 = find(',', pos1+1);
                if((pos1 > pos2) || pos1 == std::string::npos)
                        break;
                params += ", ";
        }
        return params;
}
// put comments for function, constructors and destructor
std::string CFPC::put_comment_ff(size_t pos, std::string ret_val_type,
                                 std::string fname, std::string param_list)
{
        size_t pos1 = pos;
        while('\n' != fcontent[pos1])
                pos1--;
        std::string tab = fcontent.substr(pos1+1, pos-pos1-1);
        std::string comment = "\n";
        comment += tab + "/*\n";
        comment += tab;
        if(fname == "constructor")
                comment += "*   ! constructor";
        else if(fname == "destructor")
                comment += "*   ! destructor";
        else {
                comment += "*   ! function - " + fname;
        }
        comment += '\n';
        if(ret_val_type.size()) {
                comment += tab + "*   ! ret_type - " + ret_val_type + "\n";
        }
        if(fname != "destructor") {
                comment += tab + "*   ! param_list - ";
                if(param_list.size())
                        comment += param_list;
                else
                        comment += "don't have";
                comment += '\n';
        }
        comment += tab + "*\n";
        comment += tab + "*/\n";
        comment += tab;
        fcontent.insert(pos, comment);
        return comment;
}
size_t _min(size_t a, size_t b, size_t c)
{
        size_t min = 0;
        min = (a < b) ? a : b;
        min = (min < c) ? min : c;
        return min;
}
std::string CFPC::get_first_ppp(size_t pos, size_t limit) const
{
        std::string ppp = "publicmablic";
        size_t pos1 = find("public", pos);
        size_t pos2 = find("private", pos);
        size_t pos3 = find("protected", pos);
        size_t min = _min(pos1, pos2, pos3);
        if(min > limit) return ppp;
        ppp = (min == pos1) ? "public" :
             ((min == pos2) ? "private" :
             ((min == pos3) ? "protected" : ppp ));
        return ppp;
}
std::string CFPC::put_struct_comments(size_t pos, std::string struct_type, std::string struct_name)
{
        size_t pos1 = pos, count1 = 0, count2 = 0, count3 = 0;
        std::multimap<size_t, std::string>  struct_funcs;
        std::map<size_t, std::string>  struct_vars;
        std::multimap<size_t, std::string>::iterator  mm_it;
        std::map<size_t, std::string>::iterator  m_it;
        std::vector<std::string> public_, protected_, private_;

        // get tab
        while('\n' != fcontent[pos1]) pos1--;
        size_t pos_1 = pos1;
        while(isspace(fcontent[pos_1])) pos_1++;
        std::string tab = fcontent.substr(pos1+1, pos_1-pos1-1);
        pos = pos_1;

        // make basic comment list
        public_.push_back(tab +    "*   ! public variables   -  ");  // 0
        public_.push_back(tab +    "*   ! public functions   -  ");  // 1
        protected_.push_back(tab + "*   ! protected variebles - ");  // 0
        protected_.push_back(tab + "*   ! protected functions - ");  // 1
        private_.push_back(tab +   "*   ! private variebles  -  ");  // 0
        private_.push_back(tab +   "*   ! private functions  -  ");  // 1
        std::string space    =     "*                           ";
        size_t b_comm_sz = private_[0].size();

        // processing member functions
        struct_funcs = put_struct_funcs_comments(struct_name, pos);
        size_t start = find('{', pos);
        size_t end = find_bracket_couple('{', start);
        std::string ppp_first = (struct_type == "class") ? "private" : "public";

        // arrange functions names in comment
        while( true ) {
                std::string ppp_second = get_first_ppp(start, end);
                if(ppp_first == "publicmablic") break;
                pos1 = find(ppp_second, start);
                if(pos1 > end) pos1 = end;

                for(mm_it = struct_funcs.begin(); mm_it != struct_funcs.end(); mm_it++) {
                        if(std::string::npos != public_[1].find(ppp_first)) {
                                if(((mm_it->second != struct_name) && (mm_it->second != ("~"+struct_name)))
                                                      && ((mm_it->first > start) && (mm_it->first < pos1))) {
                                        if(count1 == 3) {
                                                public_[1] += ("\n" + tab + space);
                                                count1 = 0;
                                        }
                                        public_[1] += mm_it->second + ", ";
                                        count1++;
                                }
                        }
                        else if(std::string::npos != private_[1].find(ppp_first)) {
                                if(((mm_it->second != struct_name) && (mm_it->second != ("~"+struct_name)))
                                                      && ((mm_it->first > start) && (mm_it->first < pos1))) {
                                        if(count2 == 3) {
                                                private_[1] += ("\n" + tab + space);
                                                count2 = 0;
                                        }
                                        private_[1] += mm_it->second + ", ";
                                        count2++;
                                }
                        }
                        else if(std::string::npos != protected_[1].find(ppp_first)) {
                                if(((mm_it->second != struct_name) && (mm_it->second != ("~"+struct_name)))
                                                      && ((mm_it->first > start) && (mm_it->first < pos1))) {
                                        if(count3 == 3) {
                                                protected_[1] += ("\n" + tab + space);
                                                count3 = 0;
                                        }
                                        protected_[1] += mm_it->second + ", ";
                                        count3++;
                                }
                        }
                }
                start = pos1+5;
                ppp_first = ppp_second;
        }
        // processing variables
        count1 = count2 = count3 = 0;
        struct_vars = put_struct_variables_comments(pos, struct_name);
        start = find('{', pos);
        end = find_bracket_couple('{', start);
        ppp_first = (struct_type == "class") ? "private" : "public";
        // arrange variables names in comment
        while( true ) {
                std::string ppp_second = get_first_ppp(start, end);
                if(ppp_first == "publicmablic") break;
                pos1 = find(ppp_second, start);
                if(pos1 == std::string::npos) pos1 = end;

                for(m_it = struct_vars.begin(); m_it != struct_vars.end(); m_it++) {
                        if(std::string::npos != public_[0].find(ppp_first)) {
                                if((m_it->first > start) && (m_it->first < pos1)) {
                                        if(count1 == 4) {
                                                public_[0] += ("\n" + tab + space);
                                                count1 = 0;
                                        }
                                        public_[0] += m_it->second + ", ";
                                        count1++;
                                }
                        }
                        if(std::string::npos != private_[0].find(ppp_first)) {
                                if((m_it->first > start) && (m_it->first < pos1)) {
                                        if(count2 == 4) {
                                                private_[0] += ("\n" + tab + space);
                                                count2 = 0;
                                        }
                                        private_[0] += m_it->second + ", ";
                                        count2++;
                                }
                        }
                        if(std::string::npos != protected_[0].find(ppp_first)) {
                                if((m_it->first > start) && (m_it->first < pos1)) {
                                        if(count3 == 4) {
                                                protected_[0] += ("\n" + tab + space);
                                                count3 = 0;
                                        }
                                        protected_[0] += m_it->second + ", ";
                                        count3++;
                                }
                        }
                }
                start = pos1+5;
                ppp_first = ppp_second;
        }
        // make the final comment
        std::string comment = "\n" + tab + "/*\n";
        comment += tab + "*   ! " + struct_type + "  -  " + struct_name + "\n";
        comment += tab + "* " + '\n';

        if(b_comm_sz < private_[0].size())
                comment += private_[0].substr(0, (-2+private_[0].size())) + "\n";
        if(b_comm_sz < private_[1].size())
                comment += private_[1].substr(0, (-2+private_[1].size())) + "\n";

        if(b_comm_sz < protected_[0].size())
                comment += protected_[0].substr(0, (-2+protected_[0].size())) + "\n";
        if(b_comm_sz < protected_[1].size())
                comment += protected_[1].substr(0, (-2+protected_[1].size())) + "\n";

        if(b_comm_sz < public_[0].size())
                comment += public_[0].substr(0, (-2+public_[1].size())) + "\n";
        if(b_comm_sz < public_[1].size())
                comment += public_[1].substr(0, (-2+public_[1].size())) + "\n";

        comment += tab + "*\n";
        comment += tab + "*/\n";
        comment += tab;
        fcontent.insert(pos, comment);
        return comment;
}
size_t CFPC::find_first_struct(size_t pos) const
{
        std::vector<std::string> structures;
        structures.push_back("namespace");
        structures.push_back("class");
        structures.push_back("struct");
        structures.push_back("enum");
        structures.push_back("union");
        size_t pos_m[5];
        for(int i = 0; i < 5; i++){
                pos_m[i] = find(structures[i], pos);
        }
        size_t min = _min(_min(pos_m[0], pos_m[1], pos_m[2]), pos_m[3], pos_m[4]);
        size_t pos1 = find_back('\n', min);
        size_t template_pos = find("template", pos1);
        if(template_pos < min)
                min = pos1;
        return min;
}
std::string CFPC::get_struct_type(size_t struct_pos) const
{
        size_t pos = find(' ', struct_pos);
        std::string struct_name = get_name_in_line(struct_pos, pos);
        if(struct_name == "template") {
                pos = find_bracket_couple('<', struct_pos);
                while(isspace(fcontent[++pos]));
                size_t pos1 = pos-1;
                while(isgraph(fcontent[pos]))
                        pos++;
                struct_name = get_name_in_line(pos1, pos);
        }
        return struct_name;
}
bool CFPC::is_struct_definition(size_t struct_pos) const
{
        size_t pos = fcontent.find_first_of(";{", struct_pos);
        return (fcontent[pos] == '{') ? true : false;
}
void CFPC::processing_structures(size_t struct_pos, std::string struct_name)
{
        size_t start = find_first_struct(struct_pos);
        size_t end = find_bracket_couple('{', start);
        std::string struct_nm = get_struct_name(start);
        size_t other_start = find_first_struct(start+2);
        if((other_start > start) && (other_start < end))
                processing_structures(other_start, struct_nm);
        put_funcs_comments(struct_pos, start, struct_name);
}
void CFPC::add_comments()
{
        size_t struct_end_pos = 0;
        size_t struct_pos = find_first_struct();
        if(struct_pos == std::string::npos) {
                put_funcs_comments(0, size(), "");
                return;
        }
        put_funcs_comments(0, struct_pos, "");
        struct_pos = find_first_struct();
        while(struct_pos != std::string::npos) {
                 if(is_struct_definition(struct_pos)) {
                        std::string struct_name = get_struct_name(struct_pos);
                        std::string struct_type = get_struct_type(struct_pos);
                        put_struct_comments(struct_pos, struct_type, struct_name);
                        struct_end_pos = find_bracket_couple('{', struct_pos);
                        struct_pos = struct_end_pos;
                 }
                 struct_pos = find_first_struct(++struct_pos);
        }
        put_funcs_comments(struct_end_pos, size(), "");

}



