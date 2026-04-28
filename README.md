# Sangwan's blog

Personal technical blog built with Jekyll and published with GitHub Pages.

## Requirements

- Ruby 3.3
- Bundler 2.3.11 or newer

## Setup

```sh
bundle install
```

## Local development

```sh
bundle exec jekyll serve
```

The site is served at `http://127.0.0.1:4000`.

## Build

```sh
bundle exec jekyll build
```

The generated site is written to `_site/`.

## Writing posts

Create posts under `_posts/` with the Jekyll filename format:

```text
YYYY-MM-DD-slug.md
```

Use the existing post front matter style:

```yaml
---
layout: post
title: Post title
tags: [security]
---
```

Drafts live under `_drafts/` and are excluded from the published site.

## Deployment

GitHub Pages builds the `main` branch from the repository root. The `Jekyll build` GitHub Actions workflow validates that the site can be built before relying on the Pages deployment.
