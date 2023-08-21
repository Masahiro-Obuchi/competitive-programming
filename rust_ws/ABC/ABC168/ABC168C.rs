use proconio::input;

fn main() {
    proconio::input! {
        a: f64,
        b: f64,
        h: f64,
        m: f64,
    }

    let theta = 2_f64 * std::f64::consts::PI / (60_f64 * 12_f64) * (11_f64 * m - 60_f64 * h);
    let ans_square = a * a + b * b - 2_f64 * a * b * theta.cos();
    let ans = ans_square.sqrt();
    println!("{}", ans);
}
