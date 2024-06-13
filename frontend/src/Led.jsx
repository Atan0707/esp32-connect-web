import { useState, useEffect } from 'react';

const Led = () => {
    const endpoint = "http://192.168.1.2:3000/postData"
    const getData = "http://192.168.1.2:3000/getData"
    const [data, setData] = useState(null);

    const handleOn = () => {
        console.log("on button clicked")
        fetch(endpoint, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({value: "1", counter: data.counter})
        }).then(() => {
            console.log("data sent to server");
        }).catch(error => {
            console.error("Error sending data to server:", error);
        });
    }
    const handleOff = () => {
        console.log("off button clicked")
        fetch(endpoint, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({value: "0"})
        }).then(() => {
            console.log("data sent to server");
        }).catch(error => {
            console.error("Error sending data to server:", error);
        });
    }

    const fetchData = async () => {
        const response = await fetch(getData);
        const data = await response.json();
        setData(data);
        //console.log(data)
    }

    useEffect(() => {
        fetchData(); // Fetch data on component mount
        const intervalId = setInterval(fetchData, 5000); // Poll every 5000 milliseconds (5 seconds)
        
        return () => clearInterval(intervalId); // Cleanup interval on component unmount
    }, [data]); // Re-run effect if data changes
    
    return (
        <div className="led">
            <button className="led-button" onClick={() => {handleOn()}}>RED</button>
            <button className="led-button" onClick={(() => {handleOff()})}>GREEN</button>
            {data && <div><h1>Value: {data.value}</h1></div>}
            {data && <div><h1>Counter: {data.counter}</h1></div>}
        </div>
      );
}
 
export default Led;