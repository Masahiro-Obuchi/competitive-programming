use proconio::input;
use proconio::marker::{Bytes, Chars};

fn main() {
    input! {
        s :Chars,
    }

    let ans = if s[2] == s[3] && s[4] == s[5] {
        "Yes"
    } else {
        "No"
    };

    println!("{}", ans);
}
