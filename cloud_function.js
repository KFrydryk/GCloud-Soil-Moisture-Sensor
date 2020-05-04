/**
 * Triggered from a message on a Cloud Pub/Sub topic.
 *
 * @param {!Object} event Event payload.
 * @param {!Object} context Metadata for the event.
 */
exports.helloPubSub = (event, context) => {
  const pubsubMessage = event.data;
  console.log(Buffer.from(pubsubMessage, 'base64').toString());
};



/**
 * Background Cloud Function to be triggered by PubSub.
 *
 * @param {object} event The Cloud Functions event.
 * @param {function} callback The callback function.
 */
exports.subscribe = (event, context) => {
  const BigQuery = require('@google-cloud/bigquery');
  const projectId = "cytrynka"; //Enter your project ID here
  const datasetId = "Humidity"; //Enter your BigQuery dataset name here
  const tableId = "Hum_table"; //Enter your BigQuery table name here -- make sure it is setup correctly
  const pubsubMessage = event.data;

  //console.log(Buffer.from(event, 'base64').toString());
  //var obj = JSON.parse(Buffer.from(pubsubMessage.data, 'base64').toString());
  //console.log(obj.hum);

  // Incoming data is in JSON format
  console.log(pubsubMessage);
  var incomingData = pubsubMessage ? Buffer.from(pubsubMessage, 'base64').toString() : "{'id':'na','time':'0','humidity':'-1'}";
  console.log(incomingData);
  incomingData = incomingData.replace(/'/g, '"')
  const jsonData = JSON.parse(incomingData);
  var rows = [jsonData];

  console.log(`Uploading data: ${JSON.stringify(rows)}`);

  // Instantiates a client
  const bigquery = BigQuery({
    projectId: projectId
  });

  // Inserts data into a table
  bigquery
    .dataset(datasetId)
    .table(tableId)
    .insert(rows)
    .then((foundErrors) => {
      rows.forEach((row) => console.log('Inserted: ', row));

      if (foundErrors && foundErrors.insertErrors != undefined) {
        foundErrors.forEach((err) => {
            console.log('Error: ', err);
        })
      }
    })
    .catch((err) => {
      console.error('ERROR:', err);
    });
  // [END bigquery_insert_stream]

};