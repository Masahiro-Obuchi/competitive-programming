use proconio::input;

fn main() {
    proconio::input! {
        x: i32,
    }

    if x >= 30 {
        println!("Yes");
    } else {
        println!("No");
    }
}
