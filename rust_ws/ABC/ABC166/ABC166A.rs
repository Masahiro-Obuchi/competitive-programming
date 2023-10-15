use proconio::input;
use proconio::marker::{Bytes, Chars};

fn main() {
    input! {
        s :String,
    }

    let ans = {
        match s.as_str() {
            "ABC" => "ARC",
            "ARC" => "ABC",
            _ => "",
        }
    };
    println!("{}", ans);
}
