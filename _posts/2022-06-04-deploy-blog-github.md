---
layout: post
title: Deploy own blog with Github Page
tags: [common]
---

Let's deploy own blog
with [Jekyll](https://jekyllrb.com/)
and [Remote theme](https://github.com/chesterhow/tale).

## Prerequisite
This post is based on macOS.

```sh
$ brew install ruby
$ brew link --overwrite ruby
$ gem install bundler
$ echo 'export PATH="/usr/local/opt/ruby/bin:$PATH"' >> ~/.bash_profile

# Restart Terminal
```

## Copy blog skeleton
1. Make a github repository named `{username}.github.io`
2. Copy files and directories from [tale](https://github.com/chesterhow/tale)

```sh
bitboom.github.io (master)$ ls -R
  _posts
  Gemfile
  _config.yml
  ./_pages:
    404.html  about.md  tags.html
  tale.gemspec
```

## Follow [tutorial](https://github.com/chesterhow/tale/blob/master/README.md)

> ### GitHub Pages method
> 1. Add these 2 lines in to your `Gemfile`:
> 
> ```ruby
> gem "jekyll-remote-theme"
> gem "jekyll-paginate"
> ```
> 
> 2. Install the newly added gems:
>
> ```bash
> $ bundle
> ```
> 
> 3. In `_config.yml` add these lines:
>
> ```yaml
> remote_theme: chesterhow/tale
> 
> permalink:    /:year-:month-:day/:title
> paginate:     5
>
> plugins:
>  - jekyll-paginate
>  - jekyll-remote-theme
> ```
>
> Remove any other `theme:` or `remote_theme:` lines.
>
> 4. Rename `index.md` to `index.html`. Without this, the `jekyll-paginate` gem will not work.
>
> 5. In `about.md`, change the `layout:` field to `post`:

Finally, push to your github :)
