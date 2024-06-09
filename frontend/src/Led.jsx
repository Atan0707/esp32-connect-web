

const Led = () => {
    const endpoint = "http://192.168.1.2:3000/postData"

    const handleOn = () => {
        console.log("on button clicked")
        fetch(endpoint, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({value: "1"})
        })
    }
    const handleOff = () => {
        console.log("off button clicked")
        fetch(endpoint, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({value: "0"})
        })
    }
    
    return (
        <div className="led">
            <button className="led-button" onClick={() => {handleOn()}}>RED</button>
            <button className="led-button" onClick={(() => {handleOff()})}>GREEN</button>
        </div>
      );
}
 
export default Led;