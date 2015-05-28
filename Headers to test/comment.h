#ifndef COMMENT_H
#define COMMENT_H

#include <string>
#include <vector>
#include <map>

class CFPC
{
    public:
        CFPC();
        CFPC(const std::string&);
        std::string getstr() { return fcontent; }
        std::string get_type_name(size_t) const;
        std::string get_struct_name(size_t) const;
        std::string get_struct_type(size_t) const;
        std::string get_func_params(size_t) const;
        std::string get_variable_name(size_t) const;
        std::string get_first_ppp(size_t, size_t) const;
        std::string get_name_in_line(size_t, size_t) const;
        std::string complex_type_name(size_t) const;
        std::string put_struct_comments(size_t, std::string, std::string);
        std::string put_variable_comment(size_t, std::string, std::string);
        std::string put_comment_ff(size_t, std::string, std::string, std::string);
        std::map<size_t, std::string> get_struct_names() const;
        std::map<size_t, std::string> put_struct_variables_comments(size_t, std::string);
        std::multimap<size_t, std::string> put_funcs_comments(size_t, size_t, std::string nname = "");
        std::multimap<size_t, std::string> put_struct_funcs_comments(std::string, size_t pos = 0);
        void delete_comments();
        void add_comments();
        bool is_in_string(size_t) const;
        bool is_in_comment(size_t) const;
        bool is_struct_definition(size_t) const;
        void processing_structures(size_t,
                std::string stuct_name = "");
        size_t find_bracket_couple(const char, size_t) const;
        size_t find_back(const char, size_t) const;
        size_t find_first_struct(size_t pos = 0) const;
        size_t find(const char ch, size_t pos = 0) const;
        size_t find(const char* chp, size_t pos = 0) const;
        size_t find(std::string str, size_t pos = 0) const;
        size_t size() const
                { return fcontent.size(); }
        bool is_file_opened()
                { return isFileOpened;}

    private:
        std::string fcontent;
        bool isFileOpened;
};



#endif // COMMENT_H
