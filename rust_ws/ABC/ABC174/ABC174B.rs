use proconio::input;

fn main() {
    input! {
        n: usize,
        d: f64,
        v: [(f64, f64); n],
    }

    let mut ans = 0;
    for &(x, y) in &v {
        ans += if x.hypot(y) <= d { 1 } else { 0 };
    }
    println!("{}", ans);
}
