use proconio::input;
use proconio::marker::{Bytes, Chars};

fn main() {
    input! {
        alpha :char,
    }

    let ans = if alpha.is_uppercase() { "A" } else { "a" };

    println!("{}", ans);
}
