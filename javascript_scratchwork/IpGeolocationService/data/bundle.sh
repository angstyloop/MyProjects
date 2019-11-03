# zip and push to s3
zip -r ../data.zip .
aws s3 cp ../data.zip s3://dfs-shared/IpGeolocationService/
# put a handy link on your clipboard for lambda ;)
echo "https://dfs-shared.s3.amazonaws.com/IpGeolocationService/data.zip" | pbcopy

