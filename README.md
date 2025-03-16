### Warning
- **如要使用有去背的遊戲，請參考 main_remove_background Branch**

# Candy Crush with Grass

《糖果傳奇與草》是由 syuaweu & itaouo 模仿英國網路遊戲公司 King Digital 開發的寶石方塊遊戲。糖果傳奇是以糖果爲主題；糖果傳奇與草是以草爲主題。基本規則是將三至五個一樣的草排成一線消除糖果得到分數。遊戲需要把不同的草組在一起並引爆，會產生不同效果威力。當串消草產生連擊的期間，玩家無法移動其他的草。

![Demo](https://github.com/itaouo/OOPL_CandyCrush/blob/main/Demo.gif)

## Demo
### [Demo 影片](https://www.youtube.com/watch?v=7vzftTKKj1Q)
[![Demo](https://img.youtube.com/vi/7vzftTKKj1Q/0.jpg)](https://www.youtube.com/watch?v=7vzftTKKj1Q)
### [CrushCandy（中途遇到的困難）](https://www.youtube.com/watch?v=dzQsIjr4Xtg)
[![CrushCandy](https://img.youtube.com/vi/dzQsIjr4Xtg/0.jpg)](https://www.youtube.com/watch?v=dzQsIjr4Xtg)

## 遊戲說明
特殊的草組合會形成草粿，草粿擁有強大的力量，可消一行、一列或一整個區域的草。詳細資訊如下：
![1709786306479](https://github.com/syuaweu/OOPL_CandyCrush/assets/105784560/49d234d4-6ec8-4b4b-9aef-8f7b38d541e6)

- 直草粿：消除整個直行之草
- 橫草粿：消除整個橫列之草
- 睡覺草粿：消除以其為中心周圍的八個草，兩次
- 飢餓草粿：消除與所有與其交換顏色之草

每一關樣式不同，隨機掉落草。一開始會提供五顆愛心，失敗一次扣一顆，30 分鐘補回一顆，可看**NPC廣告、syuaweu廣告**獲得愛心。

### Useful Reference
- [分工文件](https://docs.google.com/document/d/1r39oqaTfdfY3p2mdVQB-1BjzwpG-CQNy/edit?usp=drive_link&ouid=115868516804806743155&rtpof=true&sd=true)
- [LGF 框架說明手冊](https://lgf-readthedocs.readthedocs.io/zh_TW/latest/index.html)
- [LGF 框架函式庫](https://ntut-xuan.github.io/LeistungsstarkesGameFramework)
