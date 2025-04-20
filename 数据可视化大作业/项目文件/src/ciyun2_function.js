function ciyun2(data) {
    console.log(typeof data)
    if (Array.isArray(data)) {
      console.log('CSV 数据加载成功:', data);
      const comments = data.map(d => d.comment).join(' ');
      const words = tokenizeAndFilter(comments);
      const wordCounts = getWordCounts(words);
  
      // 设置字体大小的最小值和最大值
      const minFontSize = 10;  // 最小字体大小
      const maxFontSize = 40;  // 最大字体大小
      const wordData = Object.keys(wordCounts).map(word => {
        const size = wordCounts[word] * 8;
        return {
          text: word,
          size: Math.min(Math.max(size, minFontSize), maxFontSize)  // 保证字体大小在范围内
        };
      });
  
      generateWordCloud(wordData, data);
  
    } else {
      console.error("传入的数据格式不正确");
    }
  
    function tokenizeAndFilter(text) {
      const stopWords = [
        '的', '了', '在', '是', '我', '我们', '你', '你们', '它', '它们',
        '这个', '这些', '但是', '非常', '很',
        '好东西', '电影', '东西', '大家好', '不错', '一般', '哈哈哈', '呵呵', '主演', '出演'
      ];
  
      const synonymMap = {
        "非常好看": "好看",
        "很好看": "好看",
        "真好": "好",
        "好评": "好",
        "值得": "好",
        "不错": "好"
      };
  
      // 不再去除表情符号和特殊字符
      const cleanedText = text;
  
      // 使用正则表达式拆分单词，并去除停用词
      const words = cleanedText.split(/[\s，。！；？]+/)
        .map(word => word.trim())
        .filter(word => word.length > 1 && !stopWords.some(stopWord => word.includes(stopWord))) // 匹配包含停用词的词语
        .map(word => synonymMap[word] || word); // 同义词归并
  
      return words;
    }
  
  
    function getWordCounts(words) {
      return words.reduce((counts, word) => {
        counts[word] = (counts[word] || 0) + 1;
        return counts;
      }, {});
    }
  
    function generateWordCloud(wordData, commentsData) {
      const layout = d3.layout.cloud()
        .size([600, 700])
        .words(wordData)
        .padding(2)
        .rotate(function () { return 0; })
        .fontSize(function (d) { return d.size; })
        .font("Poppins")
        .on("end", drawCloud);
  
      layout.start();
  
      function drawCloud(words) {
        const svg = d3.select("#word-cloud").append("svg")
          .attr("width", layout.size()[0])
          .attr("height", layout.size()[1])
          .append("g")
          .attr("transform", "translate(" + layout.size()[0] / 2 + "," + layout.size()[1] / 2 + ")");
  
        const wordElements = svg.selectAll("text")
          .data(words)
          .enter().append("text")
          .style("font-size", function (d) { return d.size + "px"; })
          .style("font-family", "Poppins, sans-serif")
          .style("fill", function (d, i) {
            return getEmotionColor(d.text, commentsData);
          })
          .style("text-shadow", function (d) {
            return `2px 2px 4px rgba(0, 0, 0, 0.2)`;
          })
          .attr("text-anchor", "middle")
          .attr("transform", function (d) {
            return "translate(" + [d.x, d.y] + ")rotate(" + d.rotate + ")";
          })
          .text(function (d) { return d.text; })
          .on("mouseover", function (event, d) {
            d3.select(this)
              .style("font-size", (d.size * 1.2) + "px")
              .style("cursor", "pointer");
            showRandomComment(d.text, commentsData);
          })
          .on("mouseout", function (event, d) {
            d3.select(this)
              .style("font-size", d.size + "px");
            d3.select("#tooltip").style("display", "none");
          });
      }
  
      function getEmotionColor(word, commentsData) {
        // 生成固定颜色的函数，基于词语生成颜色
        function hashCode(str) {
          let hash = 0;
          for (let i = 0; i < str.length; i++) {
            hash = (hash << 5) - hash + str.charCodeAt(i);
            hash = hash & hash; // 转换为32位整数
          }
          return hash;
        }
  
        function intToRGB(value) {
          let r = (value >> 16) & 0xff;
          let g = (value >> 8) & 0xff;
          let b = value & 0xff;
          return `rgb(${r},${g},${b})`;
        }
  
        return intToRGB(hashCode(word));  // 每个词的颜色固定
      }
  
      function showRandomComment(word, commentsData) {
        const relevantComments = commentsData.filter(comment => comment.comment && comment.comment.includes(word));
  
        if (relevantComments.length > 1) {
          // 随机选择一条评论
          const randomComment = relevantComments[Math.floor(Math.random() * relevantComments.length)];
  
          const commentArea = d3.select("#comment-display");
          commentArea.html(`
                    <h3>包含 "${word}" 的评论</h3>
                    <p>${randomComment.comment}</p>
                `);
        }
      }
    }
  }

export default ciyun2;