use std::env;

//// Kinds of tokens
//enum TokenKind {
//    TkReserved, //symbol
//    TkNum,       //integer
//    TkEof,       // end of input
//}
//
//// Token type
//struct Token<'a> {
//    kind: TokenKind,
//    next: &'a Token<'a>,
//    val: i32,
//    s_token: &'a str,
//}

//#[cfg(test)]
//mod tests {
//  #[test]
//  fn 
//}


fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        println!("Inappropriate number of arguments");
        println!("# of input arguments = {}", args.len());
        return;
    }
    println!(".intel_syntax noprefix");
    println!(".globl main");
    println!("main:");
    println!("  mov rax, {}", args[1].parse::<usize>().unwrap());
    println!(" ret");
//    let mut vec = Vec::new();
//    vec.push(1);
//    vec.push(2);
}
