// Reference example: the house style this skill enforces.
// Compiles with: g++ -std=c++20 -Wall -Wextra -Wpedantic
#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

// Type in PascalCase, always a scoped enum
enum class CallPreamble : int {
    Header = 0,
    Payload,
    Checksum,
    Count
};

// Unary operator+ extracts the underlying integral value for index/counter use
[[nodiscard]] constexpr auto operator+(CallPreamble preamble_field) noexcept {
    return static_cast<std::underlying_type_t<CallPreamble>>(preamble_field);
}

static_assert(+CallPreamble::Count > 0, "Packet structure must have at least one field.");

// Class type in PascalCase
class PacketProcessor {
public:
    // Parameters in snake_case, members set through the initializer list
    explicit PacketProcessor(std::string_view client_name, int initial_id)
        : client_name_{ client_name }, id_{ initial_id } {
    }

    // Public methods in snake_case
    [[nodiscard]] int get_id() const noexcept {
        return id_;
    }

    void process_scores(const std::vector<int>& raw_scores) const {
        // STL algorithm instead of a raw loop
        const bool has_negative{ std::ranges::any_of(raw_scores, [](int score) { return score < 0; }) };
        if (has_negative) {
            log_error_("Negative score detected in batch.");
        }
    }

    void print_field_info(CallPreamble field_type) const {
        std::cout << "Field index: " << +field_type << '\n';
    }

private:
    // Private helper method ends with _
    void log_error_(std::string_view error_message) const {
        std::cout << "[LOG] " << client_name_ << ": " << error_message << '\n';
    }

    // Private data members end with _ and use default member initialization
    std::string client_name_{};
    int id_{};
};

int main() {
    const PacketProcessor processor{ "client-a", 1 };
    processor.process_scores({ 3, -2, 7 });
    processor.print_field_info(CallPreamble::Payload);
    return processor.get_id() == 1 ? 0 : 1;
}
