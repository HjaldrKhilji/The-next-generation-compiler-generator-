module;
#include <string>       // For std::string, std::string::size_type
#include <stdexcept>    // For std::runtime_error
#include <type_traits>
//i used AI to track the header files needed
module Printer;
import All_declarations;
namespace printing_tools{
    namespace helper_templates_for_options {
        namespace helpers_for_arithmetic_options {
            template <absolute_base::Is_String_Or_Numeric T>
    T read_from_string(const std::string& config, std::string::size_type* pos) {
        if constexpr (std::is_same_v<T, std::string>) {
            return read_string_from_string_at_a_position(config, pos);
        }
	 else {
            return absolute_base::read_number_from_string_at_a_position(config, pos);
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
            T convert_to_number(std::string source) {
                std::string::size_type position = 0;
		T result= read_number_from_string_at_a_position<T>(source, &position);
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

            template <absolute_base::Is_String_Or_Numeric Interal_resperentation_type>
            struct Accumulator {
                using Interal_resperentation = Interal_resperentation_type;
                Accumulator(Interal_resperentation arg) :internal_data{ arg } {}

                template <absolute_base::Numeric Interal_resperentation_of_type_converted>
                Accumulator(Accumulator<Interal_resperentation_of_type_converted> arg) {
                    interal_data = convert_to_target<Interal_resperentation>(arg.internal_data);
                }

                Interal_resperentation internal_data;
            };
            template<absolute_base::Is_String_Or_Numeric T, absolute_base::Is_String_Or_Numeric U>
            Accumulator<T>& operator+(Accumulator<T>& x, Accumulator<U> y) {
                Accumulator<T> converted_y = convert_to_target<T>(y.internal_data);
                x.internal_data += converted_y.internal_data;
                return x;
            }
            template<std::Numeric T, absolute_base::Is_String_Or_Numeric U>
            Accumulator<T>& operator-(Accumulator<T>& x, Accumulator<U> y) {
                Accumulator<T> converted_y = convert_to_target<T>(y.internal_data);
                x.internal_data -= converted_y.internal_data;
                return x;
            }
            template<std::Numeric T, absolute_base::Is_String_Or_Numeric U>
            Accumulator<T>& operator/(Accumulator<T>& x, Accumulator<U> y) {
                Accumulator<T> converted_y = convert_to_target<T>(y.internal_data);
                x.internal_data /= converted_y.internal_data;
                return x;
            }
            template<std::Numeric T, absolute_base::Is_String_Or_Numeric U>
            Accumulator<T>& operator*(Accumulator<T>& x, Accumulator<U> y) {
                Accumulator<T> converted_y = convert_to_target<T>(y.internal_data);
                x.internal_data *= converted_y.internal_data;
                return x;
            }
        }
        }
}
