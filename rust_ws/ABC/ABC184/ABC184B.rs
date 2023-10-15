use proconio::input;
use proconio::marker::{Bytes, Chars};

fn main() {
    input! {
        _ : usize,
        x: usize,
        s :String,
    }

    let mut ans = x;
    for c in s.chars() {
        if c == 'o' {
            ans += 1;
        } else if ans > 0 {
            ans -= 1;
            ans = ans.max(0);
        }
    }
    println!("{}", ans);
}
