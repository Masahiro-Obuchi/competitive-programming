use proconio::input;

fn main() {
    input! {
        n: usize,
        d: f64,
        v: [(f64, f64); n],
    }
    println!("{}", v.iter().filter(|&&(x, y)| x.hypot(y) <= d).count());
}
