echo "Installing Rust"
curl https://sh.rustup.rs -sSf | sh -s -- -y
source "$HOME/.cargo/env"

echo "Downloading checker"
cargo install hw_checker

ln -s ~/.cargo/bin/hw_checker .
mv hw_checker check

echo "Install finished. Good luck!"
