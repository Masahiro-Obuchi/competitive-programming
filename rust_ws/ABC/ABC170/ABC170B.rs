use proconio::input;

fn main() {
    proconio::input! {
        x: i32,
        y: i32,
    }

    let a = y - 2 * x;
    let b = 4 * x - y;

    if a % 2 == 0 && b % 2 == 0 && y >= 2 * x && 4 * x >= y {
        println!("Yes");
    } else {
        println!("No");
    }
}
