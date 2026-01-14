module;
#include <string>       // For std::string, std::string::size_type
#include <stdexcept>    // For std::runtime_error
#include <type_traits>
#include <memory>
#include<concepts>

//i used AI to track the header files needed
#include<utility>
module Printer;
import All_declarations;
namespace printing_tools{
    namespace helper_templates_for_options {
        namespace helpers_for_arithmetic_options {
            template <absolute_base::Is_String_Or_Numeric T>
            inline T read_from_string(const std::string& string_to_read_from, std::string::size_type* pos) {
             if constexpr (std::is_same_v<T, std::string>) {
                    return read_string_from_string_at_a_position(string_to_read_from, pos);
                }
	        else {
                    return absolute_base::read_number_from_string_at_a_position(string_to_read_from, pos);
                }
            }
            
            template <absolute_base::Is_String_Or_Numeric T, bool read_from_x_or_y>
            inline T read_from_string(const std::string& x, const std::string& y, std::string::size_type* x_pos, std::string::size_type* y_pos) {
                constexpr if (read_from_x_or_y) {
                    read_from_string(x, x_pos);
                }
                else {
                    read_from_string(y, y_pos);

                }
            }

            template <absolute_base::Numeric T>
            inline std::string convert_to_string(T source) {
                return std::to_string(source);

            }
            inline std::string convert_to_string(std::string source) {
                return source;
            }
            template <absolute_base::Numeric T>
            inline T convert_to_number(T source) {
                return source;

            }
        template <absolute_base::Numeric T>
            T convert_to_number(std::string source) {
                std::string::size_type position = 0;
		T result= absolute_base::read_number_from_string_at_a_position<T>(source, &position);
 		if (position != source.length()) {
                    throw std::runtime_error{ "number mixed with (non numeric) charactor while taking input for an option" };
                }
                return result;
               
            }
               template <absolute_base::Is_String_Or_Numeric T_dest, absolute_base::Is_String_Or_Numeric T_source> 
            inline T_dest convert_to_target(T_source source) { 
                if constexpr (std::is_same_v<T_dest, std::string>) {
                    return convert_to_string(source);
                } else {
                    return convert_to_number<T_dest>(source); 
                }
            }
            //pump (in this context) is a type of polymorphic object that has a pump_to_string function
            //in pump_to_string_function it pumps the internal data of itself to the end of a given string
            //the concept for pump is very simple:
            template<typename pump_type>
            concept pump = requires(pump_type a, std::string string_to_pump_to) {
                { a.pump(string_to_pump_to) } -> std::same_as<void>;
            };
            //the classes below are pump types, one is polymorphic, while the other is static
            class pump_static{
            public:
                //void pump(); pump need not to be defined here
            };
            class pump_polymorphic {
            public:
                virtual void pump(std::string*) = 0;
            };
           
            template <absolute_base::Is_String_Or_Numeric Internal_resperentation_type, pump base_pump>
            struct Accumulator::public base_pump {
                using Internal_resperentation = Internal_resperentation_type;
                Accumulator(Internal_resperentation arg) :internal_data{ std::move(arg) } {}

                template <absolute_base::Numeric Internal_resperentation_of_type_converted>
                Accumulator(Accumulator<Internal_resperentation_of_type_converted> arg) {
                    interal_data = convert_to_target<Internal_resperentation>(arg.internal_data);
                }
                void pump(std::string* string_to_pump_to){
                    *string_to_pump_to += std::string{internal_data};
                }
                Internal_resperentation internal_data;
            };

            template<absolute_base::Is_String_Or_Numeric T, absolute_base::Is_String_Or_Numeric U>
            Accumulator<T> operator+(Accumulator<T> x, Accumulator<U> y) {
                Accumulator<T> converted_y = convert_to_target<T>(std::move(y.internal_data));//I am using std::move() in case std::string is used
                x.internal_data += converted_y.internal_data;
                return x;
            }
            template<absolute_base::Numeric T, absolute_base::Is_String_Or_Numeric U>
            Accumulator<T> operator-(Accumulator<T> x, Accumulator<U> y) {
                Accumulator<T> converted_y = convert_to_target<T>(y.internal_data);
                x.internal_data -= converted_y.internal_data;
                return x;
            }
            template<absolute_base::Numeric T, absolute_base::Is_String_Or_Numeric U>
            Accumulator<T> operator/(Accumulator<T> x, Accumulator<U> y) {
                Accumulator<T> converted_y = convert_to_target<T>(y.internal_data);
                x.internal_data /= converted_y.internal_data;
                return x;
            }
            template<absolute_base::Numeric T, absolute_base::Is_String_Or_Numeric U>
            Accumulator<T> operator*(Accumulator<T> x, Accumulator<U> y) {
                Accumulator<T> converted_y = convert_to_target<T>(y.internal_data);
                x.internal_data *= converted_y.internal_data;
                return x;
            }
            inline bool is_char_digit(const char c) {

                if (c >= '0' && c <= '9') {
                    return true;
                }
                else {
                    return false;
                }
            }
            using helper_templates_for_options::helpers_for_arithmetic_options::pump_polymorphic;

            inline std::unique_ptr<pump_polymorphic> read_from_string(const std::string& string_to_read_from, std::string::size_type* pos) {
                template<absolute_base::Is_String_Or_Numeric T>
                using helper_templates_for_options::helpers_for_arithmetic_options::Accumulator<T, pump_polymorphic>;

                if (is_char_digit(string_to_read_from[*pos])) {
                    {
						std::string::size_type previous_pos=*pos;
                        long long int int_read= absolute_base::read_number_from_string_at_a_position<long long int>(string_to_read_from, pos);
                        if (string_to_read_from[*pos] == '.') {
                            *pos=previous_pos:
							long double double_read = absolute_base::read_number_from_string_at_a_position<long double>(string_to_read_from, pos);
                            return std::make_unique<Accumulator<long long int>>(Accumulator (double_read));

                        }
                        else {
                            return std::make_unique<Accumulator<long double>>(Accumulator(int_read));
                        }
                    }


                else {
                    return  std::make_unique<Accumulator<std:string>>(Accumulator(read_string_from_string_at_a_position(string_to_read_from, pos)));
                }
                }
        }

        }
}



