# Kubuntu Installation Note

Because I frequently mess up my operating system, plus I am so bad at debugging Linux issues, I usually just resort to **reinstallation**. So to minimize the time wasted, I make this note....

---

## set terminal shortcut

+ **System Settings -> Shortcuts -> custom shortcut -> edit -> new ....**
+ action is "konsole" (lowercase)

---

## install chrome

+ Just do it

---

## Set Chrome as default browser

+ **update**: the easy way: create a temporary html file, right click -> file extension setting -> change the priority

+ Beside the one in System Setting, run:
<code>xdg-mime default google-chrome.desktop text/html</code>

+ Check <code>~/.config/mimeapps.list</code>

---

## install packages

+ vim 
+ wget 
+ curl 
+ build-essential 
+ git 
+ zsh 
+ exfat-fuse 
+ exfat-utils 
+ tmux
+ python3-pip

```shell
sudo apt install vim wget curl build-essential git zsh exfat-fuse exfat-utils tmux python3-pip
```

---

## dotfiles

+ <code>.profile</code>
+ <code>.tmux.conf</code>
+ <code>.vimrc</code>

---

## generate ssh key and deploy

```shell 
ssh-keygen && ssh-copy-id b07902075@linux1.csie.org
```

+ add ssh key to github

---

## clone repo

+ Just do it

---

## install vscode

+ extensions: material theme(dark), git lens

---

## install chinese pinyin input method (fcitx + google-pinyin)

+ Install required packages
  ```shell
  sudo apt install fcitx-bin fcitx-config-gtk2 fcitx-frontend-qt4 fcitx-frontend-qt5 fcitx-googlepinyin fcitx-imlist fcitx-module-dbus
  ```
+ Edit <code>/etc/enviroment</code>
  ```shell
  GTK_IM_MODULE=fcitx
  QT_IM_MODULE=fcitx
  XMODIFIERS="@im=fcitx"
  ```

+ Edit <code>~/.xprofile</code>
  ```shell
  export GTK_IM_MODULE=fcitx
  export QT_IM_MODULE=fcitx
  export XMODIFIERS="@im=fcitx"
  ```

+ In **System Settings -> Regional Settings -> Language** change some setting
+ Run <code>im-config</code>
+ In **System Settings**, add fcitx to start-up routine
+ Reboot
+ In fcitx's configuration panel, add google-pinyin
+ If you cannot toggle to traditional chinese, try <code>ctrl+shift+F</code>
+ If there is error when starting fcitx, it is possibly an ibus related issue
+ If fcitx still doesn't work, try run <code>fcitx-diagnose</code>

---

## install mkdocs for this website

```shell
pip install mkdocs mkdocs-material mkdocs-macros-plugin pymdown-extensions
```

---

## install oh-my-zsh

+ go to official page on github and follow its instructions

---

## configure themes

+ Breeze, Breeze, Breeze...
+ google cursor

---
