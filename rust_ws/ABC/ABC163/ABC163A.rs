use proconio::input;

fn main() {
    proconio::input! {
        r: f64,
    }
    println!("{}", 2_f64 * std::f64::consts::PI * r);
}
