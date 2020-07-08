
// Kinds of tokens
enum TokenKind {
    TkReserved, //symbol
    TkNum,       //integer
    TkEof,       // end of input
}

// Token type
struct Token<'a> {
    kind: TokenKind,
    next: &'a Token<'a>,
    val: i32,
    s_token: &'a str,
}

fn main() {
    let mut vec = Vec::new();
    vec.push(1);
    vec.push(2);
    println!("Hello, Rust.");
}
