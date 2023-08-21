use proconio::input;

fn main() {
    proconio::input! {
        a: i32,
    }
    println!("{}", a + a * a + a * a * a);
}
