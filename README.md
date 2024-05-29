# Unreal Injects

Unreal Engine のための Dependency Injection プラグイン。 Unreal Engine の Object / Property リフレクションの基づいて、依存性の解決と注入、インスタンスのライフサイクル管理を行う。

このプラグインは現在開発中で、安定版はリリースされていません。

## Features

- [x] Blueprint / C++ で実装された UObject と UInterface の依存性の解決と注入
- [x] UPROPERTY Injection
- [x] UFUNCTION Injection
- [x] World / GameInstance などの Gameplay Framework と連動したマネージドコンテナの作成
- [x] コンテナが生成した UObject へのライフサイクルイベント(Construct, Cleanup, ...)の通知
- [x] 親子関係を持つコンテナの作成
- [ ] 非同期の依存性解決と注入

## Installation

1. プロジェクト直下に `Plugins` ディレクトリを作成
2. このリポジトリを `Plugins/UnrealInjects` にクローン

## Documentation
work in progress

## Examples
work in progress