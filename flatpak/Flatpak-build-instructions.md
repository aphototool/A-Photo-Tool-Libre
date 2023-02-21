build:
```
flatpak-builder build-dir me.ahola.aphototoollibre.yml
```

test:
```
flatpak-builder --user --install --force-clean build-dir me.ahola.aphototoollibre.yml
```
```
flatpak run me.ahola.aphototoollibre
```
test more:
```
flatpak-builder --repo=repo --force-clean build-dir me.ahola.aphototoollibre.yml
```
```
flatpak --user remote-add --no-gpg-verify tutorial-repo repo
```
```
flatpak --user install tutorial-repo me.ahola.aphototoollibre
```

uninstall:
```
flatpak uninstall me.ahola.aphototoollibre
```

