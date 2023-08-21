use proconio::input;

fn main() {
    proconio::input! {
        n: i32,
        x: i32,
        t: i32,
    }
    println!("{}", (n + x - 1) / x * t);
}
