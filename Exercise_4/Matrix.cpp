export module matrix;
import std;

export class Imatrix{
    int rows;
    int cols;
    std::vector<int> elems;

    //private helper that throws outside matrix
    void check_index(int x, int y) const{
        if(x < 0 || x >= rows || y < 0 || y >= cols){
            throw std::out_of_range(std::format("Imatrix: index({}, {}) out of range for a {}x{} matrix", x, y, rows, cols));
        }
    }
    void check_same_size(const Imatrix& other, const std::string_view op) const{
        if(rows != other.rows || cols != other.cols){
            throw std::invalid_argument(std::format("operator{}: dimensions do not match ({}x{} vs {}x{})", op, rows, cols, other.rows, other.cols));
        }
    }
    std::size_t index(int x, int y) const{
        return static_cast<std::size_t>(x) * static_cast<std::size_t>(cols) + static_cast<std::size_t>(y);
    }
    static int checked_dim(int n){
        if(n < 0){
            throw std::invalid_argument(std::format("Imatrix: negative dimension"));
        }
        return n;
    }
    


public:
    //Requirement 1 default operator
    Imatrix() : rows(0), cols(0) {}
    Imatrix(int x, int y) : rows{checked_dim(x)}, cols{checked_dim(y)}, elems(static_cast<std::size_t>(x) * static_cast<std::size_t>(y), 0){}
    
    //requirement 2 Assignment operator and copy and move constructors.
    Imatrix(const Imatrix&) = default;
    Imatrix& operator=(const Imatrix&) = default;

    Imatrix(Imatrix&& other) noexcept : rows(other.rows), cols(other.cols), elems(std::move(other.elems)){
        other.rows = 0;
        other.cols = 0;
    }
    Imatrix& operator=(Imatrix&& other) noexcept {
        if(this != &other){
            elems = std::move(other.elems);
            rows = other.rows;
            cols = other.cols;
            other.rows = 0;
            other.cols = 0;
        }
        return *this;
    }

    //Requirement 3 Subscripting
    int& operator()(int x, int y){
        check_index(x, y);
        return elems[index(x, y)];
    }
    const int& operator()(int x, int y) const{
        check_index(x, y);
        return elems[index(x, y)];
    }


    //requirement 4 +, *, /, -, and %, yielding a new Imatrix.
    Imatrix operator+(const Imatrix& other) const{
        check_same_size(other, "+");

        Imatrix res(rows, cols);
        for(std::size_t i = 0; i< elems.size();++i){
            res.elems[i] = elems[i] + other.elems[i];
        }
        return res;
    }

    Imatrix operator*(const Imatrix& other) const{
        check_same_size(other, "*");
        Imatrix res(rows, cols);
        for(std::size_t i = 0; i< elems.size();++i){
            res.elems[i] = elems[i] * other.elems[i];
        }
        return res;
    }

    Imatrix operator/(const Imatrix& other) const{
        check_same_size(other, "/");
        
        Imatrix res(rows, cols);
        for(std::size_t i = 0; i< elems.size();++i){
            if(other.elems[i] == 0) {
                throw std::domain_error("operator/: division by zero");
            }
            res.elems[i] = elems[i] / other.elems[i];
        }
        return res;
    }

    Imatrix operator-(const Imatrix& other) const{
        check_same_size(other, "-");
        
        Imatrix res(rows, cols);
        for(std::size_t i = 0; i< elems.size();++i){
            res.elems[i] = elems[i] - other.elems[i];
        }
        return res;
    }
    Imatrix operator%(const Imatrix& other) const{
        check_same_size(other, "%");

        Imatrix res(rows, cols);
        for(std::size_t i = 0; i< elems.size();++i){
            if(other.elems[i] == 0) {
                throw std::domain_error("operator%: division by zero");
            }
            res.elems[i] = elems[i] % other.elems[i];
        }
        return res;
    }
    //requirement 5 Move(x,y)
    void Move(std::pair<int, int> x, std::pair<int, int> y){
        const auto [fx, fy] = x;
        const auto [tx, ty] = y;
        check_index(fx, fy);
        check_index(tx, ty);
        if(x == y) return;
        elems[index(tx, ty)] = elems[index(fx, fy)];
        elems[index(fx, fy)] = 0;
    }
    
    //requirement 6 Row(n)
    std::vector<int> Row(int n) const {
        if (n < 0 || n >= rows) {
            throw std::out_of_range{std::format("Row({})", n)};
        };
        std::vector<int> res;
        res.reserve(static_cast<std::size_t>(cols));
        for(int y = 0; y < cols; ++y){
            res.push_back(elems[index(n, y)]);
        }
        return res;
    }
    //Requirement 7 Column(n)
        std::vector<int> Column(int n) const {
        if (n < 0 || n >= cols) {
            throw std::out_of_range{std::format("Column({})", n)};
        };
        std::vector<int> res;
        res.reserve(static_cast<std::size_t>(rows));
        for(int x = 0; x < rows; ++x){
            res.push_back(elems[index(x, n)]);
        }
        return res;
    }
    int Rows() const {return rows;}
    int Cols() const {return cols;}
};
export void Print(const Imatrix& m){
    for(int x = 0; x < m.Rows(); ++x){
        for(int y = 0; y < m.Cols(); ++y){
            std::print("{} ", m(x,y));
        }
        std::println();
    }
};

/* Part two - concepts */

/*Chess piece */
export struct Chess_piece{
    enum class Kind {None, Pawn, Knight, Bishop, Rook, Queen, King};
    enum class Color {White, Black};
    Kind kind{Kind::None};
    Color color{Color::White};
    bool empty() const {return kind == Kind::None;}
    bool operator==(const Chess_piece&) const = default;
};

export template<typename T> requires std::formattable<T, char> std::string To_string(const T& value){
    return std::format("{}", value);
}
export std::string To_string(const Chess_piece& piece){
    constexpr std::string_view letters = ".PNBRQK";
    const char c = letters[static_cast<std::size_t>(piece.kind)];
    return std::string(1, piece.color == Chess_piece::Color::White ? c : std::tolower(c));
}

/* Part two - Matrix<T> */
export template<typename T>
concept Elem = std::default_initializable<T> && std::copyable<T>;

export template <typename T>
concept Addable = requires(const T& a, const T& b) {
    { a + b } -> std::convertible_to<T>;
};

export template <typename T>
concept Arithmetic = requires(const T& a, const T& b) {
    { a - b } -> std::convertible_to<T>;
    { a * b } -> std::convertible_to<T>;
    { a / b } -> std::convertible_to<T>;
};

export template <typename T>
concept Modulable = requires(const T& a, const T& b) {
    { a % b } -> std::convertible_to<T>;
};


export template<Elem T>
class Matrix {
    int rows;
    int cols;
    std::vector<T> elems;
    void check_index(int x, int y) const{
        if(x < 0 || x >= rows || y < 0 || y >= cols){
            throw std::out_of_range(std::format("Matrix: index({}, {}) out of range for a {}x{} matrix", x, y, rows, cols));
        }
    }
    void check_same_size(const Matrix& other, const std::string_view op) const{
        if(rows != other.rows || cols != other.cols){
            throw std::invalid_argument(std::format("operator{}: dimensions do not match ({}x{} vs {}x{})", op, rows, cols, other.rows, other.cols));
        }
    }
    std::size_t index(int x, int y) const{
        return static_cast<std::size_t>(x) * static_cast<std::size_t>(cols) + static_cast<std::size_t>(y);
    }
    static int checked_dim(int n){
        if(n < 0){
            throw std::invalid_argument(std::format("Matrix: negative dimension"));
        }
        return n;
    }

    template<typename Op>
    Matrix zip(const Matrix& other, Op op, const std::string_view op_name) const {
        check_same_size(other, op_name);
        Matrix res(rows, cols);
        for(std::size_t i = 0; i< elems.size();++i){
            res.elems[i] = op(elems[i], other.elems[i]);
        }
        return res;
    }
public:
    // Requirement 1
    Matrix() : rows(0), cols(0) {}
    Matrix(int x, int y) : rows{checked_dim(x)}, cols{checked_dim(y)}, elems(static_cast<std::size_t>(x) * static_cast<std::size_t> (y)) {}

    // Requirement 2
    Matrix(const Matrix&) = default;
    Matrix& operator=(const Matrix&) = default;
    ~Matrix() = default;

    Matrix(Matrix&& other) noexcept : rows(other.rows), cols(other.cols), elems(std::move(other.elems)){
        other.rows = 0;
        other.cols = 0;
    }
    Matrix& operator=(Matrix&& other) noexcept {
        if(this != &other){
            elems = std::move(other.elems);
            rows = other.rows;
            cols = other.cols;
            other.rows = 0;
            other.cols = 0;
        }
        return *this;
    }
    // Requirement 3
    T& operator()(int x, int y){
        check_index(x, y);
        return elems[index(x, y)];
    }
    const T& operator()(int x, int y) const{
        check_index(x, y);
        return elems[index(x, y)];
    }

    // Requirement 4
    Matrix operator+(const Matrix& other) const requires Addable<T> {
        return zip(other, std::plus<T>{}, "+");
    }
    Matrix operator-(const Matrix& other) const requires Arithmetic<T> {
        return zip(other, std::minus<T>{}, "-");
    }
    Matrix operator*(const Matrix& other) const requires Arithmetic<T> {
        return zip(other, std::multiplies<T>{}, "*");
    }
    Matrix operator/(const Matrix& other) const requires Arithmetic<T> {
        return zip(other, std::divides<T>{}, "/");
    }
    Matrix operator%(const Matrix& other) const requires Modulable<T> {
        return zip(other, std::modulus<T>{}, "%");
    }

    //Requirement 5
    void Move(std::pair<int, int> x, std::pair<int, int> y){
        const auto [fx, fy] = x;
        const auto [tx, ty] = y;
        check_index(fx, fy);
        check_index(tx, ty);
        if(x == y) return;
        elems[index(tx, ty)] = std::move(elems[index(fx, fy)]);
        elems[index(fx, fy)] = T{};
    }

    //Requirement 6
    std::vector<T> Row(int n) const {
        if (n < 0 || n >= rows) {
            throw std::out_of_range{std::format("Row({})", n)};
        };
        std::vector<T> res;
        res.reserve(static_cast<std::size_t>(cols));
        for(int y = 0; y < cols; ++y){
            res.push_back(elems[index(n, y)]);
        }
        return res;
    }
    //Requirement 7
    std::vector<T> Column(int n) const {
        if (n < 0 || n >= cols) {
            throw std::out_of_range{std::format("Column({})", n)};
        };
        std::vector<T> res;
        res.reserve(static_cast<std::size_t>(rows));
        for(int x = 0; x < rows; ++x){
            res.push_back(elems[index(x, n)]);
        }
        return res;
    }

    int Rows() const {return rows;}
    int Cols() const {return cols;}
};
export template<typename T>
void Print(const Matrix<T>& m){
    for(int x = 0; x < m.Rows(); ++x){
        for(int y = 0; y < m.Cols(); ++y){
            std::print("{} ", To_string(m(x,y)));
        }
        std::println();
    }
};   
