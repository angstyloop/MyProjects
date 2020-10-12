var title = document.title
var html = '<p style="font-size: 60px;">'+title+'</p>'
var newWindow = window.open("","Test","width=500,height=300,scrollbars=1,resizable=1")
newWindow.document.open()
newWindow.document.write(html)

