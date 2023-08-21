use proconio::input;

fn main() {
    proconio::input! {
        sx: f64,
        sy: f64,
        gx: f64,
        gy: f64,
    }

    println!("{}", sx - sy * (gx - sx) / (-gy - sy));
}
