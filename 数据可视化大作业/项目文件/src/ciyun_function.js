function ciyun(data) {
    console.log(typeof data)
    if (Array.isArray(data)) {
      console.log("传入的数据是一个数组，直接处理数据");
      const comments = data.map(d => d.评论).join(' ');
  
      const words = tokenizeAndFilter(comments);
  
      const wordCounts = getWordCounts(words);
  
      const wordData = Object.keys(wordCounts).map(word => ({
        text: word,
        size: wordCounts[word] * 10
      }));
  
      generateWordCloud(wordData, data);
  
    } else {
      console.error("传入的数据格式不正确");
    }
  
    function tokenizeAndFilter(text) {
      const stopWords = ['的', '了', '在', '是', '我', '我们', '你', '你们', '它', '它们', '这个', '这些', '但是', '非常', '很'];
      return text.split(/[\s，。！；？]+/)
        .filter(word => !stopWords.includes(word) && word.length > 1);
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
        .rotate(function () { return (Math.random() > 0.5 ? 90 : 0); })
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
            showCommentWithHighestLikes(d.text, commentsData);
          })
          .on("mouseout", function (event, d) {
            d3.select(this)
              .style("font-size", d.size + "px");
            d3.select("#tooltip").style("display", "none");
          });
      }
  
      function getEmotionColor(word, commentsData) {
        const ratingMap = {
          "力荐": "#4CAF50",
          "推荐": "#2196F3",
          "还行": "#607D8B",
          "较差": "#F44336",
          "很差": "#FF5722"
        };
  
        let color = "#607D8B";
        const relevantComments = commentsData.filter(comment => comment.评论.includes(word));
  
        if (relevantComments.length > 0) {
          const firstRating = relevantComments[0]['五星评分'];
          color = ratingMap[firstRating] || "#607D8B";
        }
  
        return color;
      }
  
      function showCommentWithHighestLikes(word, commentsData) {
        const relevantComments = commentsData.filter(comment => comment.评论.includes(word));
  
        if (relevantComments.length > 0) {
          const highestLikedComment = relevantComments.sort((a, b) => b.点赞数 - a.点赞数)[0];
  
          const commentArea = d3.select("#comment-display");
          commentArea.html(` 
                    <h3>包含 "${word}" 的评论</h3>
                    <p><strong>点赞数:</strong> ${highestLikedComment.有用数}</p>
                    <p>${highestLikedComment.评论}</p>
                `);
        }
      }
    }
  }

export default ciyun;