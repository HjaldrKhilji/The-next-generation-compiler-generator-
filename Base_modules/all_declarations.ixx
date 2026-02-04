module;
// Basic types and I/O
#include <string>
#include <iostream>
#include <sstream>

// Containers
#include <vector>
#include <deque>
#include <map>
#include <stack>

// Functional and Utility
#include <functional> // For std::function and std::reference_wrapper
#include <utility>    // For std::move and std::pair
#include <iterator>   // For std::reverse_iterator
#include <type_traits>// For std::underlying_type_t and std::is_arithmetic_v

// C++20 Concepts support
#include <concepts>   // For std::same_as, std::convertible_to, etc.
// I used AI to keep track of the headers needed


export module All_declarations;//for c++ noobs, including myself, the module name dosent have to be the same as file name.

    namespace common_functions {
       tempate<typename arg...>
       void escape_string(std::string* input_string,
            const std::vector < std::string >& strings_to_be_replaced,
            const std::vector < std::function < std::string*, std::string::size_type*, args...> >
            std::function < std::string*, std::string::size_type*, args...>,
			args... arguments);
        std::string read_identifier(std::istringstream* line_stream);
        template <typename T>
        concept Numeric = std::is_arithmetic_v<T>;

        template<Numeric T>
        T  read_number_from_string_at_a_position(const std::string& source, std::string::size_type* position);
        std::string read_string_from_string_at_a_position(const std::string& source_string, std::string::size_type* position);
        inline bool convert_to_bool(std::string str);
        inline bool convert_to_bool(const std::string& input_str, std::string::size_type* position);
    }
    namespace common_concepts {

        template<typename T, typename type_to_stream = std::string>
        concept OutputStream = requires(T & os, type_to_stream & s) {

            { os } -> std::same_as<std::ostream&>;
            { os << s } -> std::same_as<std::ostream&>;

        };
        template<typename T, typename type_to_stream = std::string>
        concept InputStream = requires(T & is, type_to_stream & s) {

            { is } -> std::same_as<std::istream&>;
            { is >> s } -> std::same_as<std::istream&>;

        };

        template<typename T, typename type_to_stream = std::string>
        concept Streamable = OutputStream<T, type_to_stream> || InputStream<T, type_to_stream>;
        template<typename Pointer_type, typename value_pointer_to_by_pointer>
       concept SmartPointer = requires(Pointer_type ptr) {
            
            { *ptr } -> std::convertible_to<value_pointer_to_by_pointer&>;  // Can be dereferenced to get T
            { ptr.operator->() } -> std::same_as<value_pointer_to_by_pointer*>; // Has pointer-like access for the "->" operator
            { static_cast<bool>(ptr) };           // Can be checked for null
        };
        template<typename T> 
        concept Is_String_Or_Numeric = requires(T & is) {
            { is }->std::same_as<std::string>
        } || common_functions::Numeric<T>;

        

    }

    namespace low_level_memory_management {
        
        template<
            common_concepts::Streamable T,
            template<typename> typename PtrTemplate
        >
        requires common_concepts::SmartPointer<PtrTemplate<T>, T>
        class Streamable_manager {
        public:
            using T_ptr= PtrTemplate<T>;

            // Use std::move to support move-only types like std::unique_ptr
            explicit Streamable_manager(PtrTemplate<T_ptr> holder) : holder(holder) {}

            void switchToNewStream(T* new_stream) {
                // Automatically cleans up the old stream if this was the last reference
                *holder = T_ptr{ new_stream };
                std::cout << "[Manager] Stream updated.\n";
            }
            T& operator*() const {
                return **holder;
            }
            Streamable_manager(PtrTemplate<T_ptr> a):holder{a}{}
            //the rest of constructors(and destructor) are default, hence you dont need to worry about "under the hood" memory management .
        private:
            PtrTemplate<T_ptr> holder;
        };
        
    }
    export namespace absolute_base {
        using common_concepts::Streamable;
        using common_functions:convert_to_bool;
        using common_concepts::Is_String_Or_Numeric;
        using low_level_memory_management::Streamable_manager;
        using common_functions::Numeric;

        using common_functions::escape_string;
        using common_functions::read_identifier;
        using common_functions::read_integer_from_string_at_a_position;
        using common_functions::read_string_from_string_at_a_position;
        enum class Settings_for_semantical_rules : short {
            NONE = 0,
            check_exist = 1,
            check_dont_exist = 2,
            check_atleast = 4,
            check_exact = 8
        };

        
        using Underlying_type_of_settings_for_semantical_rules = std::underlying_type_t<Settings_for_semantical_rules>;

      
        //     ~Functions for enum class Settings_for_semantical_rules~ 

        bool operator&&(Settings_for_semantical_rules a, Settings_for_semantical_rules b) {
            using underlying_t = Underlying_type_of_settings_for_semantical_rules;
            return static_cast<bool>(
                static_cast<underlying_t>(a) & static_cast<underlying_t>(b)
                );
        }

        Settings_for_semantical_rules operator|(Settings_for_semantical_rules a, Settings_for_semantical_rules b) {
            using underlying_t = Underlying_type_of_settings_for_semantical_rules;
            return static_cast<Settings_for_semantical_rules>(
                static_cast<underlying_t>(a) | static_cast<underlying_t>(b)
                );
        }

        Settings_for_semantical_rules& operator|=(Settings_for_semantical_rules& a, Settings_for_semantical_rules b) {
            a = a | b;
            return a;
        }
        Settings_for_semantical_rules operator^(Settings_for_semantical_rules a, Settings_for_semantical_rules b) {
            using underlying_t = Underlying_type_of_settings_for_semantical_rules;
            return static_cast<Settings_for_semantical_rules>(
                static_cast<underlying_t>(a) ^ static_cast<underlying_t>(b)
                );
        }

        Settings_for_semantical_rules& operator^=(Settings_for_semantical_rules& a, Settings_for_semantical_rules b) {
            a = a ^ b;
            return a;
        }
        template<typename config>
        struct Semantical_analyzer_config_entry {
            config the_pattern_to_check;
            Settings_for_semantical_rules all_settings;

            unsigned int minimum_amount_of_matches;
            unsigned int maximum_amount_of_matches;
        };
		void check_pattern(const std::vector < Semantical_analyzer_config_entry<config> >& semantical_checks, const std::string& text);

        struct is_moved_from_bool {
            
            bool is_moved_from=false;
            is_moved_from_bool(is_moved_from_bool&& a) noexcept :is_moved_from{a.is_moved_from } { a.is_moved_from =true}
            explicit operator bool() const{
                //returns false if not moved from and true if moved from
                return is_moved_from;
            }

        };
        template<typename config>
        struct Non_terminal_name_entry {
            
            uint64_t name;
            union {
                config pattern;
                config output_config_data;//dont even think of making this one const! (it wont allow branching)
                //keep the type of both as std::string !!!!!
            
            };
            using sub_entry_type = std::vector < std::reference_wrapper < Non_terminal_name_entry<config> >>; 
            sub_entry_type sub_entries;
            using all_semantical_analysis_rules_type = std::vector < std::vector < Semantical_analyzer_config_entry<config>>>;
            is_moved_from_bool is_moved_from;
            all_semantical_analysis_rules_type all_semantical_analysis_rules;
            ~Non_terminal_name_entry() {}//for supressing warnings of implicitly deleted destructor
            Non_terminal_name_entry(const Non_terminal_name_entry<config>&) = default;
            Non_terminal_name_entry(Non_terminal_name_entry<config>&) = default;
            Non_terminal_name_entry(Non_terminal_name_entry<config>&&) = default;//moving, however, wont be so I allowed it
            Non_terminal_name_entry(std::string a, std::string b, sub_entry_type c, all_semantical_analysis_rules_type<config> d) :name{a}, pattern{b}, sub_entries{c}, all_semantical_analysis_rules{d}
            {}
        };
        template<typename config>
        class Siblings {
        private:
            //keep the data members are lightweight as possible because I am passing them by value in other parts of the code.
            using representation_type = std::reference_wrapper < std::vector < std::reference_wrapper < Non_terminal_name_entry >>>;
            //the default constructor would obviously initialize the members as expected
            representation_type Representation;
            //basically representation_type is the sub entries type.
            //like initialize it with sub entries of a non terminal entry
            using representation_type_without_reference_wrapper = std::vector < std::reference_wrapper < Non_terminal_name_entry<config> >>;
            using sub_entries_type = std::vector < std::reference_wrapper < Non_terminal_name_entry<config> >>;
            int current_sibling_index;
        public:
            inline std::vector<Semantical_analyzer_config_entry<config>>::iterator begin() {
                return this->get_semantic_rules_for_current_sibling().begin();
            }
            inline std::vector<Semantical_analyzer_config_entry<config>>::iterator end() {
                return this->get_semantic_rules_for_current_sibling().end();
            }
            Siblings(representation_type a, int b) : Representation{ a }, current_sibling_index{ b } {}
            representation_type_without_reference_wrapper& operator()() {
                return Representation.get();
            }

            int get_current_sibling_index() {
                return current_sibling_index;
            }
            void set_current_sibling_index(int a) {
                current_sibling_index = a;
            }
            Non_terminal_name_entry<config> const & get_current_sibling() {
                return (Representation.get())[current_sibling_index].get();
            }
            bool check_if_current_sibling_has_no_children() {
                return get_current_sibling().sub_entries.empty();

            }
            sub_entries_type get_sub_entries_of_current_subling() {
                return get_current_sibling().sub_entries;
            }
            std::vector<absolute_base::Semantical_analyzer_config_entry<config>>& get_semantic_rules_for_current_sibling() {
                return get_current_sibling().all_semantical_analysis_rules[get_current_sibling_index()];
            }
            inline void dig_one_generation_in(){
            Representation.get() = current_generation.get_sub_entries_of_current_subling();
            set_current_sibling_index(current_generation().size() - 1);

            }


        };
        template<typename config>
        class All_non_terminal_entries { 
            //the traversal begin() returns the last element, and end() returns one before the
                        //first element, thats because the last entry will be the one that the user of this
                        //structure will PROBABLY traverse from.
        public:  void add_non_term_symbol_name(uint64_t name)=0;//this adds the non term symbol only to fast traversal list
               void add_non_term_pattern_for_newest_entry(config pattern)=0;//this adds the newest element to the fast search map as well, after it has enter the pattern into the newest element traversal list
               config& get_pattern_of_nested_non_term_symbol_pattern(uint64_t sub_symbol_name)=0;//this gets the pattern for a name, from the map, and the reason I say for nested non term symbol is because it is generally used for finding the symbols for nested non term symbols
               void add_nested_non_term_symbol_to_the_newest_entry(uint64_t sub_symbol_name)=0;//this just takes a string, which it assumes to be the name of a non term symbol, and finds the non term symbol(from the map), and adds it's reference into the nested non term symbol list of the newest entry
               void add_semantic_rule_for_newest_sub_entry(const Semantical_analyzer_config_entry<config>&& semantical_rule_entry)=0;//this is my favorite(I spend 4 days on making this), so all it does is that it finds the latest entry from the fast traversal list, then finds the latest sub entry in that, and adds this semantic rule entry for that sub entry 
               void print_all_content()=0;
               Non_terminal_name_entry<config>& get_current_non_term_entry(int index)=0;//simply gets the non term entry at the given index
               Non_terminal_name_entry<config>& get_current_nested_non_term_entry(int index_for_nested_non_term, int index)=0;//simple gets the nested non term entry at the given indexes, or in [index][index_for_nested_non_term]
             
              using ReverseIt = std::reverse_iterator<std::deque < Non_terminal_name_entry<config> >::iterator>;
              using ReverseIt = std::reverse_iterator<std::deque < Non_terminal_name_entry<config> >::iterator>;
             ReverseIt end() final{
                  auto iterator = list_of_all_non_term_entries_for_fast_traversal.rbegin();
                  for (; bool{ iterator->is_moved_from }; iterator++);
                      return iterator;
              }
              ReverseIt begin() final{
                  return list_of_all_non_term_entries_for_fast_traversal.rend();

              }ReverseIt physical_end() final{
                  return list_of_all_non_term_entries_for_fast_traversal.rbegin();

              }
              using Iterator_for_list_of_entries = std::deque<Non_terminal_name_entry<config>>::iterator;
             Non_terminal_name_entry<config>* find_entry(std::string) = 0;

             void add_a_child_to_entry(Non_terminal_name_entry<config>* entry_to_be_added_to, absolute_base::Non_terminal_name_entry<config>&& entry_to_add) = 0;
             void add_semantic_rule_to_entry(Non_terminal_name_entry<config>* entry_to_be_added_to,  Semantical_analyzer_config_entry<config>&& semantical_rule_entry, int sibling_index, int semantic_entry_index) = 0;
             void remove_entry( Non_terminal_name_entry<config>* entry_to_add) = 0;
             void remove_semantic_rule_of_entry(Non_terminal_name_entry<config>* entry_to_remove_from, int sibling_index, int index_of_semantic_rule) = 0;
                 
		        private:
		            //this type isnt a Value oriented type, but rather a reference type to a large
		            // global "database" of the entire program.
		            //the two types and aliases are just implementation details, please ignore it tho, if you arent implementing this class.
		            
		            using Reference_to_Non_terminal_name_entry = std::reference_wrapper < Non_terminal_name_entry<config> >;
		            
		
		            //it should be obvious that the map is for fast lookups, and deque is for fast traversal:
		            
		            std::map < uint64_t,
		                Iterator_for_list_of_entries
		                > map_for_fast_retrival_of_entries;
		            std::deque < Non_terminal_name_entry<config> > list_of_all_non_term_entries_for_fast_traversal;

        };
       
        template<typename config>
        void dig_to_the_leaves_of_the_family_tree(Siblings<config> current_generation, std::stack< Siblings<config>>* family_tree) {
        //the algorithm gurrentied that only leaf nodes are the current_generation after this function is called
        //PERFECT!!

            do {
                if(( current_generation.get_current_sibling_index() - 1 )=>0){
                
                (*family_tree).push(Siblings<config>{ current_generation(), current_generation.get_current_sibling_index() - 1 });
                }
                current_generation.dig_one_generation_in();
            }while(current_generation.check_if_current_sibling_has_no_children() != true);



        }
        
        
        class Base_printer {
        public:
            virtual bool print(std::string string_to_output) = 0;
            virtual ~Base_printer() = 0;//has to be virtual since its a public base class that CAN BE USED AS A HANDLE


        };
        
        
    };
