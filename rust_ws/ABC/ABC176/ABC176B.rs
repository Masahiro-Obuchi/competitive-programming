use proconio::input;
use proconio::marker::{Bytes, Chars};

fn main() {
    input! {
         n: Bytes,
    }

    let mut count: u32 = 0;
    for b in n {
        count += (b - b'0') as u32;
    }
    let ans = if count % 9 == 0 { "Yes" } else { "No" };
    println!("{}", ans);
}
