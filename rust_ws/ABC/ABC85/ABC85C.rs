use proconio::input;
use proconio::marker::{Bytes, Chars};

fn main() {
    input! {
        n: i32,
        s: i32,
    }

    for x in 0..=n {
        for y in 0..=n - x {
            let z = n - x - y;
            if z < 0 {
                continue;
            }
            if 10000 * x + 5000 * y + 1000 * z == s {
                println!("{} {} {}", x, y, z);
                return;
            }
        }
    }
    println!("-1 -1 -1");
}
