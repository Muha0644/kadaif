# Kadaif - HoI4 Modding Tool
**Kadaif** is a cross-platform tool meant to help you make mods for Hearts of Iron IV. With VSCode and all it's extensions, managing a massive codebase is easy, but it would be a lot easier if the code was well written to begin with.<br>
Most HoI 4 modders are not expert developers, so obvisouly the code they write will not be perfect (pretty shit in other words). That's where kadaif comes in!<br>
<br>
Kadaif aims to automate most boring things like making events, focuses, and gfx files. You can simply use the GUI and then generate everything. You don't even have to copy and paste, you can just specify the file you want to use and it will even detect existing stuff so you can edit it. When you combine this with VSC and it's extensions, your mod will have better performance and the code will look neater. 

### Kadaif has cool features such as:
* automatic code generation
* your mom
* convert all `.dds` files to BC3/DTX5 (to save space)
<br><br>

# Installation
 Download a binary from the [releases tab](https://github.com/Muha0644/kadaif/releases), or compile from source if you want the latest and greatest. If you use a Mac... I'm really sorry for you. Here, have a cookie 🍪<br>
 Anyway here is how to compile from source if you want to:<br>
 ### <em>Windows</em>
  You can check out the official [install guide](https://doc.qt.io/qt-6/get-and-install-qt.html) for Qt6 which requires making an account.
### <em>Linux</em>
 Arch: `sudo pacman -Syu qt6-base qt6-doc qt6-tools qt6-translations`<br>
 Debian: [idk, you can make a PR to add the command]<br>
 RedHat: [same]<br>

### <em>Compile Qt from source</em>
 You can also compile Qt from [source](https://www.qt.io/offline-installers). This will, of course, be a lot more difficult and take a long time to compile, but it's worth it if you don't want to make an account you will never need. To compile Qt6 from source, follow this guide for [linux](https://doc.qt.io/qt-6/linux-building.html) or this one for [windows](https://doc.qt.io/qt-6/windows-building.html). If you followed the guide correctly you should already have all the dependencies you need.
 ## Install Kadaif
  Now that you have everything you need, download and install kadaif:
* `git clone https://github.com/Muha0644/kadaif`
* `cd kadaif`
* `cmake .`
* `cmake --build .`


Yeah you're probabbly better off just downloading a premade binary...

Of course if you use Arch Linux there is an AUR package: `kadaif-git` or `kadaif-bin` (not yet, this text is a placeholder)
<br><br>

# Sponsored section
This section is an advertisement for a mod i'm working on. Check out **Waves of the Danube (WotD)** - A TNO submod that aims to add content for Hungary, Croatia, Romania, and eventually the rest of the Balkans.

Naturally, if you want Waves of the Danube to come out sooner. A larger team would be needed. If you have good GFX skills, writing skills or god bless us, have coding knowledge in HOI4 (or C++). Please join [our discord](https://discord.gg/kXsaS6jzsH)!<br><br>

# License
This code is licensed under GPL 3.0, that means you can do whatever you want with it, modify it, distibute it, you can even use it for commercial purposes. The only thing you can't do is sublicense it, I.E. use a license other than GPL 3.0.

You have to include a copy of the [GPL 3.0 license](https://github.com/Muha0644/kadaif/blob/master/LICENSE) in any code that derives from this code. You also _have_ to include install instructions and a list of all significant changes as well as a reference to the original code (basically just use git and you should be fine).<br><br>

# Contributing
If you want to add a feature or fix a bug, just fork this repo, fix it, and submit a pull request. Your name will be added in the credits.<br>
I recommed using QtCreator, because thats what I used, and also because it's just got everything you need. It's a great C++ IDE in general, and working with Qt is less painful using it.
<br><br>

# Credits
* muha0644
* monkebanana8
* bog1sart
* et. al.
