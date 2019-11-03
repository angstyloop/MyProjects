/* 
 * sean@dfstudio 
 * 20190807
 * Queries the GeoLite2-City MaxMind database using the ip in the query string.
*/
const Reader = require('@maxmind/geoip2-node').Reader
exports.handler = function(event, context, callback) {
    Reader.open('./GeoLite2-City.mmdb').then(reader => {
        let resBody = reader.city(event.queryStringParameters.ip)
        let res = {
            statusCode: 200,
            body: resBody
        }
        callback(null, res)
    })
}
