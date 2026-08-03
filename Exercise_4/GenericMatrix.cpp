import std;
import matrix;

static int failures = 0;

void check(bool cond, const std::string_view what){
    std::println("{}  {}", cond ? "[ ok ] " : "[FAIL]", what);
    if(!cond){
        ++failures;
    }
}

template<typename E, typename F>
void check_throws(E e, F f, const std::string_view what){
    try{
        f();
        check(false, what);
    }catch(const E&){
        check(true, what);
    }catch(...){
        check(false, what);
    }
}

void test_matrix_part_one(){
    std::println("=== Part 1: Imatrix ===");
    Imatrix empty;
    check(empty.Rows() == 0 && empty.Cols() == 0, "default constructor creates 0x0 matrix");
    Imatrix a(2, 3);
    check(a(0,0) == 0 && a(1,2) == 0, "elements default at 0");

    //requirement 3 test
    a(0,0) = 1; a(0,1) = 2; a(0,2) = 3;
    a(1,0) = 4; a(1,1) = 5; a(1,2) = 6;
    check(a(1,1) == 5, "m(x,y) is assignable");
    Print(a);


    Imatrix b(2, 3);
    for (int x = 0; x < 2; ++x){
        for (int y = 0; y < 3; ++y){
            b(x,y) = 2;
        }
    }
    check((a+b)(1,1) == 7, "operator+ works");
    check((a*b)(1,1) == 10, "operator* works");
    check((a-b)(1,1) == 3, "operator- works");
    check((a/b)(1,1) == 2, "operator/ works");
    check((a%b)(1,1) == 1, "operator% works");
    check(a(1,1) == 5, "a is unchanged after operations");

    //requirement 5 test
    Imatrix mv = a;
    mv.Move({0,0}, {1,0});
    check(mv(1,0) == 1 && mv(0,0) == 0, "Move works");
    mv.Move({0,1}, {0,1});
    check(mv(0,1) == 2, "Move to same position does nothing");

    //requirement 6 + 7 test
    check((a.Row(1) == std::vector<int>{4,5,6}), "Row works");
    check((a.Column(2) == std::vector<int>{3,6}), "Column works");
    
    //requirement 2 test
    Imatrix copy = a;
    copy(0,0) = 99;
    check(a(0,0) == 1, "copy construction is a deep copy");
    
    Imatrix assigned(1,1);
    assigned = a;
    check(assigned(1,2) == 6 && assigned.Rows() == 2, "copy assignment works");

    Imatrix moved = std::move(copy);
    check(moved(0,0) == 99, "move construction works");
    check(copy.Rows() == 0 && copy.Cols() == 0, "moved-from matrix report itself as 0x0");

    //error tests
    // check_throws<std::out_of_range>([&]{a(2,0);}, "row index out of range throws");
    // check_throws<std::out_of_range>([&]{a(0,-1);}, "negative column index throws");
    // check_throws<std::out_of_range>([&]{a.Row(7);}, "Row out of range throws");
    // check_throws<std::out_of_range>([&]{a.Column(8);}, "Column out of range throws");
    // check_throws<std::out_of_range>([&]{a.Move({0,0}, {9,9});}, "Move to out of range throws");
    // check_throws<std::invalid_argument>([&]{a + Imatrix(3,3);}, "mismatched dimensions throws");
    // check_throws<std::invalid_argument>([&]{Imatrix bad(-1, 3);}, "negative dimension throws");
    // check_throws<std::domain_error>([&]{a % Imatrix(2,3);}, "division by zero throws");

}
void test_matrix_part_two_int(){
    std::println("=== Part 2: Matrix<int> ===");
    Matrix<int> a(2, 2);
    a(0, 0) = 10; a(0, 1) = 20; a(1, 0) = 30; a(1, 1) = 40;

    Matrix<int> b(2, 2);
    for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 2; ++y) b(x, y) = 3;

    check((a + b)(1, 1) == 43, "Matrix<int> operator+ works");
    check((a % b)(1, 1) == 1, "Matrix<int> operator% works");
    check((a.Row(0) == std::vector<int>{10, 20}), "Matrix<int> Row works");
    Print(a);

}

void test_matrix_part_two_str(){
    std::println("=== Part 2: Matrix<string> ===");
    Matrix<std::string> m(2, 2);
    check(m(0, 0).empty(), "elements default to T{} (the empty string)");

    m(0, 0) = "Hello";
    m(0, 1) = "World";

    Matrix<std::string> suffix(2, 2);
    suffix(0, 0) = "!";
    check((m + suffix)(0, 0) == "Hello!", "operator+ concatenates strings");

    m.Move({0, 1}, {1, 1});
    check(m(1, 1) == "World" && m(0, 1).empty(),
          "Move leaves source in default state");
    Print(m);

    // uncommenting the line below will cause a compile-time error, as expected:
    // auto bad1 = m - suffix;
    // auto bad2 = m % suffix;

}

void test_chess_piece(){
    std::println("=== Part 2: Chess_piece ===");
    
    using K = Chess_piece::Kind;
    using C = Chess_piece::Color;

    Matrix<Chess_piece> board(8, 8);
    check(board(0, 0).empty(), "a fresh board is all empty squares");

    board(0, 4) = Chess_piece{K::King, C::Black};
    board(7, 4) = Chess_piece{K::King, C::White};
    for (int y = 0; y < 8; ++y) board(6, y) = Chess_piece{K::Pawn, C::White};

    board.Move({6, 4}, {4, 4});
    check(board(4, 4).kind == K::Pawn, "pawn placed on its new square");
    check(board(6, 4).empty(), "square it left is empty again");
    Print(board);

    const auto rank = board.Row(6);
    check(rank.size() == 8 && rank[4].empty(), "Row works for a non-arithmetic type");

    //uncommenting the line below will cause a compile-time error, as expected:
    // auto nonsense = board + board;   // <- would fail on Addable<Chess_piece>

}

int main(){
    try{
        test_matrix_part_one();  
        test_matrix_part_two_int();
        test_matrix_part_two_str();
        test_chess_piece();
    }
    catch(const std::exception& e){
        std::println("unexpected exception: {}", e.what());
        return 2;
    }
    std::println("\n{}", failures == 0 ? "All tests passed" : std::format("{} test(s) failed", failures));
    return failures == 0 ? 0 : 1;
}