use proconio::input;

fn main() {
    proconio::input! {
        n: i32,
    }

    if n % 2 == 1 {
        println!("Black");
    } else {
        println!("White");
    }
}
