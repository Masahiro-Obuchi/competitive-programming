use proconio::input;
use proconio::marker::{Bytes, Chars};

fn main() {
    input! {
        s: Chars,
    }

    for i in 0..s.len() {
        if (i % 2 == 0) ^ s[i].is_ascii_lowercase() {
            println!("No");
            return;
        }
    }
    println!("Yes");
}
