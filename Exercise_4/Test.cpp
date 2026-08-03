import std;
import matrix;

namespace {

int failures = 0;

void check(bool ok, std::string_view what) {
    std::println("{}  {}", ok ? "[ ok ]" : "[FAIL]", what);
    if (!ok) ++failures;
}

// Runs f and reports whether it threw the expected exception type.
template <typename E, typename F>
void check_throws(F f, std::string_view what) {
    try {
        f();
        check(false, what);
    } catch (const E&) {
        check(true, what);
    } catch (...) {
        check(false, what);
    }
}

void test_imatrix() {
    std::println("--- Part 1: Imatrix ---");

    Imatrix empty;
    check(empty.Rows() == 0 && empty.Cols() == 0, "default construction gives a 0x0 matrix");

    Imatrix a(2, 3);
    check(a(0, 0) == 0 && a(1, 2) == 0, "elements default to 0");

    // requirement 3: assigning through the subscript operator
    a(0, 0) = 1; a(0, 1) = 2; a(0, 2) = 3;
    a(1, 0) = 4; a(1, 1) = 5; a(1, 2) = 6;
    check(a(1, 1) == 5, "m(x,y) is assignable");
    Print(a);

    Imatrix b(2, 3);
    for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 3; ++y) b(x, y) = 2;

    check((a + b)(1, 1) == 7, "operator+");
    check((a - b)(1, 1) == 3, "operator-");
    check((a * b)(1, 1) == 10, "operator*");
    check((a / b)(1, 1) == 2, "operator/");
    check((a % b)(1, 1) == 1, "operator%");
    check(a(1, 1) == 5, "arithmetic leaves the operands untouched");

    // requirement 5
    Imatrix mv = a;
    mv.Move({0, 0}, {1, 0});
    check(mv(1, 0) == 1 && mv(0, 0) == 0, "Move copies the value and zeroes the source");
    mv.Move({0, 1}, {0, 1});
    check(mv(0, 1) == 2, "Move onto itself is a no-op");

    // requirements 6 and 7
    check((a.Row(1) == std::vector<int>{4, 5, 6}), "Row(n)");
    check((a.Column(2) == std::vector<int>{3, 6}), "Column(n)");

    // requirement 2
    Imatrix copy = a;
    copy(0, 0) = 99;
    check(a(0, 0) == 1, "copy construction is a deep copy");

    Imatrix assigned(1, 1);
    assigned = a;
    check(assigned(1, 2) == 6 && assigned.Rows() == 2, "copy assignment");

    Imatrix moved = std::move(copy);
    check(moved(0, 0) == 99, "move construction transfers the elements");
    check(copy.Rows() == 0 && copy.Cols() == 0, "moved-from matrix reports itself as 0x0");

    // range errors
    check_throws<std::out_of_range>([&] { a(2, 0); }, "row index out of range throws");
    check_throws<std::out_of_range>([&] { a(0, -1); }, "negative index throws");
    check_throws<std::out_of_range>([&] { a.Row(7); }, "Row out of range throws");
    check_throws<std::out_of_range>([&] { a.Column(7); }, "Column out of range throws");
    check_throws<std::out_of_range>([&] { a.Move({0, 0}, {9, 9}); }, "Move out of range throws");
    check_throws<std::invalid_argument>([&] { return a + Imatrix(3, 3); },
                                       "mismatched dimensions throw");
    check_throws<std::invalid_argument>([&] { Imatrix bad(-1, 2); },
                                       "negative dimension throws");
    check_throws<std::domain_error>([&] { return a / Imatrix(2, 3); },
                                    "division by zero throws");
}

void test_matrix_int() {
    std::println("\n--- Part 2: Matrix<int> ---");

    Matrix<int> a(2, 2);
    a(0, 0) = 10; a(0, 1) = 20; a(1, 0) = 30; a(1, 1) = 40;

    Matrix<int> b(2, 2);
    for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 2; ++y) b(x, y) = 3;

    check((a + b)(1, 1) == 43, "Matrix<int> operator+");
    check((a % b)(1, 1) == 1, "Matrix<int> operator% (integral only)");
    check((a.Row(0) == std::vector<int>{10, 20}), "Matrix<int> Row");
    Print(a);
}

void test_matrix_string() {
    std::println("\n--- Part 2: Matrix<string> ---");

    Matrix<std::string> m(2, 2);
    check(m(0, 0).empty(), "elements default to T{} (the empty string)");

    m(0, 0) = "Hello";
    m(0, 1) = "World";

    Matrix<std::string> suffix(2, 2);
    suffix(0, 0) = "!";
    check((m + suffix)(0, 0) == "Hello!", "operator+ concatenates strings");

    m.Move({0, 1}, {1, 1});
    check(m(1, 1) == "World" && m(0, 1).empty(),
          "Move leaves the source in the default state");
    Print(m);

    // Enabling either line below is a compile-time error naming the concept that
    // failed -- Subtractable<std::string> / Modulable<std::string> -- rather than a
    // wall of template instantiation noise:
    // auto bad1 = m - suffix;
    // auto bad2 = m % suffix;
}

void test_matrix_chess() {
    std::println("\n--- Part 2: Matrix<Chess_piece> ---");

    using K = Chess_piece::Kind;
    using C = Chess_piece::Color;

    Matrix<Chess_piece> board(8, 8);
    check(board(0, 0).empty(), "a fresh board is all empty squares");

    board(0, 4) = Chess_piece{K::king, C::black};
    board(7, 4) = Chess_piece{K::king, C::white};
    for (int y = 0; y < 8; ++y) board(6, y) = Chess_piece{K::pawn, C::white};

    board.Move({6, 4}, {4, 4});                      // e2-e4
    check(board(4, 4).kind == K::pawn, "the pawn arrived on its new square");
    check(board(6, 4).empty(), "the square it left is empty again");
    Print(board);

    const auto rank = board.Row(6);
    check(rank.size() == 8 && rank[4].empty(), "Row works for a non-arithmetic type");

    // Matrix<Chess_piece> compiles even though Chess_piece has no arithmetic at all,
    // because the operators are constrained individually rather than on the class.
    // auto nonsense = board + board;   // <- would fail on Addable<Chess_piece>
}

}  // namespace

int main() {
    try {
        test_imatrix();
        test_matrix_int();
        test_matrix_string();
        test_matrix_chess();
    } catch (const std::exception& e) {
        std::println("unexpected exception escaped a test: {}", e.what());
        return 2;
    }

    std::println("\n{}", failures == 0 ? "all tests passed"
                                       : std::format("{} test(s) failed", failures));
    return failures == 0 ? 0 : 1;
}