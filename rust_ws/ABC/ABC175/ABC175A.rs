use proconio::input;
use proconio::marker::{Bytes, Chars};

fn main() {
    input! {
        s :String,
    }

    let mut ans = 0;
    let mut count = 0;
    for c in s.chars() {
        match c {
            'R' => count += 1,
            'S' => count = 0,
            _ => panic!(),
        }
        ans = ans.max(count);
    }

    println!("{}", ans);
}
