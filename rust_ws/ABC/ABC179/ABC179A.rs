use proconio::input;
use proconio::marker::{Bytes, Chars};

fn main() {
    input! {
        s :Chars,
    }

    let ans = if s[s.len() - 1] == 's' {
        format!("{}es", s.iter().collect::<String>())
    } else {
        format!("{}s", s.iter().collect::<String>())
    };

    println!("{}", ans);
}
