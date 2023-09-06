use crate::{console::console, info, print, println, warn};

pub struct Shell {
    buffer: [char; 256],
    buffer_index: usize,
}

impl Shell {
    pub const fn new() -> Self {
        Self {
            buffer: ['\0'; 256],
            buffer_index: 0,
        }
    }
    fn match_string(&self, s: &str) -> bool {
        let mut i = 0;
        for c in s.chars() {
            if self.buffer[i] != c {
                return false;
            }
            i += 1;
        }
        true
    }
    pub fn handle_command(&mut self) {
        if self.match_string("help") {
            println!(" @help     - print this help message");
            println!(" @panic    - panic the kernel");
            println!(" @warn     - print a warning");
        } else if self.match_string("panic") {
            println!(" @panic");
            panic!("shell panic by manual request");
        } else if self.match_string("warn") {
            println!(" @warn");
            warn!("(handle_command): this is a warning");
        } else {
            println!(" @unknown command");
        }
    }
}

pub fn main_loop() -> ! {
    // init a new shell
    let mut shell = Shell::new();
    loop {
        print!("stellaros> ");
        loop {
            let c = console().read_char();
            match c {
                '\0' => (),
                '\n' => {
                    print!("\n");
                    if (shell.buffer_index == 0) {
                        break;
                    }
                    info!("(main_loop): command entered: ");
                    // print buffer
                    print!(" CMD = [ ");
                    for i in 0..shell.buffer_index {
                        print!("{}", shell.buffer[i]);
                    }
                    print!(" ]\n");
                    // handle command
                    shell.handle_command();
                    // clear buffer
                    for i in 0..shell.buffer_index {
                        shell.buffer[i] = '\0';
                    }
                    shell.buffer_index = 0;
                    break;
                }
                _ => {
                    print!("{}", c);
                    shell.buffer[shell.buffer_index] = c;
                    shell.buffer_index += 1;
                }
            }
        }
    }
}
